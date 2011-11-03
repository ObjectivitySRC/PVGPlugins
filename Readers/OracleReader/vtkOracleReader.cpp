#include "vtkOracleReader.h"
#include "StringUtilities.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include <vtksys/ios/sstream>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

#include <occi.h>
#include <oratypes.h>
#include <stdio.h>

using namespace oracle::occi;

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


	Connection	*con;
	Statement	*stmt;
	Environment *env;
	ResultSet	*res;

	string db;
	string username;
	string password;
	string Px;
	string Py;
	string Pz;
	string propName;

	string line;
	vector<string> lineSplit;

	// username
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	username = lineSplit[1];

	// password
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	password = lineSplit[1];

	// oracle EZCONNECT string
	getline(inFile, line);
	StringUtilities::split(line, lineSplit, "=");
	db = lineSplit[1];

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
	try
	{
		// create a default environment
		env = Environment::createEnvironment(Environment::DEFAULT);
		// connect to the database and create a connection
		con = env->createConnection(username, password, db);
		
		vector<double> xValue, yValue, zValue;

		// select all X
		string sql = "select " + Px + ", " + Py + ", " + Pz + ", " + propName + " from sigg_ly_pdist_collares";
		stmt = con->createStatement(sql);
		res = stmt->executeQuery();
		

		//// get the list of columns
		//MetaData emptab_metaData = con->getMetaData("sigg_ly_pdist_collares", MetaData::PTYPE_TABLE);
		//vector<MetaData> listOfCols = emptab_metaData.getVector(MetaData::ATTR_LIST_COLUMNS);
		//fprintf(log, "%u", listOfCols.size());
		//fclose(log);
		//// assign the proper column indicies
		//int xIndex, yIndex, zIndex, propIndex;
		//for (unsigned i = 0; i < listOfCols.size(); i++)
		//{
		//	/*************************************/
		//	string columnName = listOfCols.at(i).getString(MetaData::ATTR_NAME);
		//	/*************************************/
		//
		//	if (columnName==Px)
		//	{
		//		xIndex = i;
		//	}
		//	if (columnName==Py)
		//	{
		//		yIndex = i;
		//	}
		//	if (columnName==Pz)
		//	{
		//		zIndex = i;
		//	}
		//	if (columnName==propName)
		//	{
		//		propIndex = i;
		//	}
		//}
		vtkPoints* outPoints = vtkPoints::New();
		vtkCellArray* outVerts = vtkCellArray::New();
		while(res->next())
		{
			double pt[3];
			// add 1 since they start at 1, not 0
			pt[0] = res->getDouble(1);
			pt[1] = res->getDouble(2);
			pt[2] = res->getDouble(3);

			double propValue = res->getDouble(4);

			vtkIdType pid = outPoints->InsertNextPoint(pt);
			outVerts->InsertNextCell(1);
			outVerts->InsertCellPoint(pid);

			propArray->InsertNextValue(propValue);
		}

		output->SetPoints(outPoints);
		output->SetVerts(outVerts);
		output->GetPointData()->AddArray(propArray);

		// close everything down
		if (res)
		{
			stmt->closeResultSet(res);
		}
		if (stmt)
		{
			con->terminateStatement(stmt);
		}
		if (env)
		{
			Environment::terminateEnvironment(env);
		}
	}
	catch(oracle::occi::SQLException &e)
	{
         vtkErrorMacro(""<<e.what());
	}
	
	return 1;
}