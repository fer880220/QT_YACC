#ifndef IDS_H
#define IDS_H

#include <QString>
typedef enum {simbError = -1,  simbId , simbFlecha , simbPComa ,
              simbEos,
             simbStart , simbS , simbReglas , simbR , simbDer
             }TSimbolo ;

static QString NombresSImb[]={	"id",	"flecha", "pComa",
                           "eos",	"Start", "S",	"Reglas",	"R","Der"
};

static QString CadSalidas[]={"\n \n    //CONJUNTO DE ESTADOS\n", "I", " = "};
#endif // IDS_H
