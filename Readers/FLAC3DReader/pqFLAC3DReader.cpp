#include "pqFLAC3DReader.h"


//________________________________________________________________________
pqFLAC3DReader::pqFLAC3DReader(pqProxy* pxy, QWidget* p)
: pqLoadedFormObjectPanel(":/PVGReaders/pqFLAC3DReader.ui", pxy, p)
{
	this->linkServerManagerProperties();
}


//________________________________________________________________________
pqFLAC3DReader::~pqFLAC3DReader()
{
}


//________________________________________________________________________
void pqFLAC3DReader::accept()
{
	pqLoadedFormObjectPanel::accept();
}


//________________________________________________________________________
void pqFLAC3DReader::linkServerManagerProperties()
{
	// parent class hooks up some of our widgets in the ui
	pqLoadedFormObjectPanel::linkServerManagerProperties();
}