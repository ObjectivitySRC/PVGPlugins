#include "vtkFLAC3DReader.h"

#include "vtkUnstructuredGrid.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include <vtksys/ios/sstream>
#include <sstream>

#include <vtkStringList.h>
#include <vtkStdString.h>
#include <vtkStringArray.h>
#include <vtkIntArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include "StringUtilities.h"


vtkCxxRevisionMacro(vtkFLAC3DReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkFLAC3DReader);

//_________________________________________________________________________
vtkFLAC3DReader::vtkFLAC3DReader()
{
	this->FileName = NULL;
	this->SetNumberOfInputPorts(0);
}

//_________________________________________________________________________
vtkFLAC3DReader::~vtkFLAC3DReader()
{
	this->SetFileName(0);
}

//_________________________________________________________________________
void vtkFLAC3DReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent <<  "FileName: "
		<< (this->FileName ? this->FileName : "(none)") << "\n";
}

//_________________________________________________________________________
int vtkFLAC3DReader::CanReadFile( const char* fname )
{
	return 1;	
}

//_________________________________________________________________________
int vtkFLAC3DReader::RequestInformation(vtkInformation* request,
									  vtkInformationVector** inputVector,
									  vtkInformationVector* outputVector)
{
	return 1;
}


//_________________________________________________________________________
int vtkFLAC3DReader::RequestData(vtkInformation* request,
							   vtkInformationVector** inputVector,
							   vtkInformationVector* outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	// Make sure we have a file to read.
	if(!this->FileName)  {
		vtkErrorMacro("No file name specified.  Cannot open.");
		return 0;
	}
	if(strlen(this->FileName)==0)  {
		vtkErrorMacro("File name is null.  Cannot open.");
		return 0;
	}


	ifstream inFile;
	inFile.open(this->FileName, ios::in);
	if(!inFile)
	{
		vtkErrorMacro("File Error: cannot open file: "<< this->FileName);
		return 0;
	}

	vtkPoints* outPoints = vtkPoints::New();

	vtkIdType ids[8];
	string line;
	vector<string> lineSplit;

	// skip the headers
	while(!inFile.eof())
	{
		getline(inFile, line);
		if(line.size() == 0) continue;
		if(line[0] == '*') continue;
		if(line[0] == 'G')
			break;
	}

	// read the GRIDPOINTS section
	while(line.substr(0,2) == "G ")
	{
		StringUtilities::split(line, lineSplit, " ");

		vtkIdType id;
		double pt[3];
		id = atoi(lineSplit[1].c_str());
		pt[0] = atof(lineSplit[2].c_str());
		pt[1] = atof(lineSplit[3].c_str());
		pt[2] = atof(lineSplit[4].c_str());

		outPoints->InsertNextPoint(pt);

		if(inFile.eof()) break;

		do
		{
			getline(inFile, line);
		}while((line.size() == 0 || line[0] == '*') &! inFile.eof());

		if(line.size() == 0 || line[0] == '*')
			break;
	}

	if(line.size() == 0 || line[0] == '*' || inFile.eof())
	{
		output->SetPoints(outPoints);
		outPoints->Delete();
		return 1;
	}

	vtkUnstructuredGrid* temp= vtkUnstructuredGrid::New();

	// read the ZONES section
	while(line.substr(0,2) == "Z ")
	{
		StringUtilities::split(line, lineSplit, " ");

		if(lineSplit[1] == "B8")
		{
			vtkIdType id = atoi(lineSplit[2].c_str());

			// see FLAC3D documentation for hexahedron topology
			ids[0] = atoi(lineSplit[3].c_str()) -1;
			ids[1] = atoi(lineSplit[4].c_str()) -1;
			ids[2] = atoi(lineSplit[7].c_str()) -1;
			ids[3] = atoi(lineSplit[5].c_str()) -1;
			ids[4] = atoi(lineSplit[6].c_str()) -1;
			ids[5] = atoi(lineSplit[9].c_str()) -1;
			ids[6] = atoi(lineSplit[10].c_str()) -1;
			ids[7] = atoi(lineSplit[8].c_str()) -1;

			temp->InsertNextCell(VTK_HEXAHEDRON,8,ids);
		}

		if(inFile.eof()) break;

		do
		{
			getline(inFile, line);
		}while((line.size() == 0 || line[0] == '*') &! inFile.eof());

		if(line.size() == 0 || line[0] == '*')
			break;
	}

	temp->SetPoints(outPoints);
	output->ShallowCopy(temp);
	outPoints->Delete();

	if(line.size() == 0 || line[0] == '*' || inFile.eof())
		return 1;

	vtkStringArray* groupNameArray = vtkStringArray::New();
	groupNameArray->SetName("Group Name");
	groupNameArray->SetNumberOfValues(temp->GetNumberOfCells());

	vtkIntArray* groupIdArray = vtkIntArray::New();
	groupIdArray->SetName("Group Id");
	groupIdArray->SetNumberOfValues(temp->GetNumberOfCells());

	// in case some zones doesn't belong to a group
	// if it's sure that all zones belong to a group, this
	// loop should be removed to save time
	//for(vtkIdType i=0; i<temp->GetNumberOfCells(); ++i)
	//{
	//	groupNameArray->SetValue(i, "");
	//	groupIdArray->SetValue(i,0);
	//}

	int groupId = 0;
	while(line.substr(0,7) == "ZGROUP ")
	{
		StringUtilities::split(line, lineSplit, " ");
		string groupName = lineSplit[1].substr(1,lineSplit[1].size()-2);

		while(true)
		{
			do
			{
				getline(inFile, line);
			}while((line.size() == 0 || line[0] == '*') &! inFile.eof());

			if(line.size() == 0 || line[0] == '*' || inFile.eof() || line.substr(0,6) == "ZGROUP")
				break;
			
			StringUtilities::split(line, lineSplit, " ");
			for(vector<string>::iterator it = lineSplit.begin(); it != lineSplit.end(); ++it)
			{
				int zone = atoi(it->c_str()) -1;
				groupNameArray->SetValue(zone, groupName);
				groupIdArray->SetValue(zone, groupId);
			}
		}
		++groupId;
	}

	output->GetCellData()->AddArray(groupNameArray);
	output->GetCellData()->AddArray(groupIdArray);

	

	return 1;
}
