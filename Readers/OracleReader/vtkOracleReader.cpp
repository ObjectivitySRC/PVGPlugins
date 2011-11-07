#include "vtkOracleReader.h"
#include "StringUtilities.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include <vtksys/ios/sstream>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

#include "ocilib.h"
#include <stdio.h>

using namespace std;

//_________________________________________________________________________
vtkCxxRevisionMacro(vtkOracleReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkOracleReader);

//_________________________________________________________________________
vtkOracleReader::vtkOracleReader()
{
	this->FileName = NULL;
	this->SetNumberOfInputPorts(0);
}

//_________________________________________________________________________
vtkOracleReader::~vtkOracleReader()
{
	this->SetFileName(0);
}

//_________________________________________________________________________
void vtkOracleReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent <<  "FileName: "
		<< (this->FileName ? this->FileName : "(none)") << "\n";
}

//_________________________________________________________________________
int vtkOracleReader::CanReadFile( const char* fname )
{
	return 1;
}

//_________________________________________________________________________
int vtkOracleReader::RequestInformation(vtkInformation* request,
									  vtkInformationVector** inputVector,
									  vtkInformationVector* outputVector)
{
	return 1;
}

	//_________________________________________________________________________
int vtkOracleReader::RequestData(vtkInformation* request,
							   vtkInformationVector** inputVector,
							   vtkInformationVector* outputVector)
{
	FILE *log;
	log = fopen("C:/oracle_log.txt", "w");
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	// open the <filename>.oracle parameters file
	ifstream inFile;
	inFile.open(this->FileName, ios::in);
	if(!inFile)
	{
		vtkErrorMacro("File Error: cannot open file: "<< this->FileName);
		return 0;
	}


	OCI_Connection* con;
	OCI_Statement* stmt;
	OCI_Resultset* res;

	string db;
	string user;
	string passwd;
	string tableName;
	string Px;
	string Py;
	string Pz;
	string propName;

	string line;
	vector<string> lineSplit;

	// username
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	user = lineSplit[1];

	// password
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	passwd = lineSplit[1];

	// oracle EZCONNECT string
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	db = lineSplit[1];

	// get the table name
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	tableName = lineSplit[1];

	// get the x values column name
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	Px = lineSplit[1];

	// get the y values column name
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	Py = lineSplit[1];

	// get the z values column name
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	Pz = lineSplit[1];

	// get the property name
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	propName = lineSplit[1];

	vtkDoubleArray* propArray = vtkDoubleArray::New();
	propArray->SetName(propName.c_str());
	if (!OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT))
	{
		vtkErrorMacro("Could not initialize environment");
		return EXIT_FAILURE;
	}
	con = OCI_ConnectionCreate(db.c_str(), user.c_str(), passwd.c_str(), OCI_SESSION_DEFAULT);
	stmt = OCI_StatementCreate(con);
	
	vector<double> xValue, yValue, zValue;

	// select desired values
	string sql = "select " + Px + ", " + Py + ", " + Pz + ", " + propName + " from " + tableName;
	if (!OCI_ExecuteStmt(stmt, sql.c_str()))
	{
		vtkErrorMacro("Could not execute SQL statement");
		return EXIT_FAILURE;
	}
	
	// Get the result set and get the desired data
	vtkPoints* outPoints = vtkPoints::New();
	vtkCellArray* outVerts = vtkCellArray::New();
	res = OCI_GetResultset(stmt);
	while(OCI_FetchNext(res))
	{
		double pt[3];
		// add 1 since they start at 1, not 0
		pt[0] = OCI_GetDouble(res, 1);
		pt[1] = OCI_GetDouble(res, 2);
		pt[2] = OCI_GetDouble(res, 3);

		double propValue = OCI_GetDouble(res, 4);

		vtkIdType pid = outPoints->InsertNextPoint(pt);
		outVerts->InsertNextCell(1);
		outVerts->InsertCellPoint(pid);

		propArray->InsertNextValue(propValue);
	}

	output->SetPoints(outPoints);
	output->SetVerts(outVerts);
	output->GetPointData()->AddArray(propArray);

	OCI_Cleanup();
	
	return 1;
}