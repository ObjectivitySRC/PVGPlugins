#ifndef _pqFLAC3DReader_h
#define _pqFLAC3DReader_h

#include "pqLoadedFormObjectPanel.h"
#include "pqComponentsExport.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkSmartPointer.h"

#include <QMap>
#include <QTableWidget>
#include <QDockWidget>

class QComboBox;
class vtkSMStringVectorProperty;

class pqFLAC3DReader : public pqLoadedFormObjectPanel {
  Q_OBJECT
public:
  /// constructor
  pqFLAC3DReader(pqProxy* proxy, QWidget* p = NULL);
  /// destructor
  ~pqFLAC3DReader();

   virtual void accept();
    
protected:
  /// populate widgets with properties from the server manager
  virtual void linkServerManagerProperties();
};

#endif

