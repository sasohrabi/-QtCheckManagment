/*
Name: QtRpt
Version: 1.5.0
Programmer: Aleksey Osipov
e-mail: aliks-os@ukr.net
2012-2015
*/

#ifndef RPTPAGEOBJECT_H
#define RPTPAGEOBJECT_H

#include <qtrptnamespace.h>
#include <RptBandObject.h>
#include <RptFieldObject.h>

using namespace QtRptName;

class QtRPT;
class RptBandObject;
class RptFieldObject;

class RptPageObject
{
    friend class QtRPT;
public:
    RptPageObject();
    ~RptPageObject();
    int pageNo;
    int orientation;
    int ph;
    int pw;
    int ml;
    int mr;
    int mt;
    int mb;
    void addBand(RptBandObject *band);
    RptBandObject *getBand(BandType type);
    RptFieldObject *findFieldObjectByName(QString name);
    QList<RptBandObject*> bandList;
private:
    void setProperty(QtRPT *qtrpt, QDomElement docElem);
};
Q_DECLARE_METATYPE(RptPageObject)
QDebug operator<<(QDebug dbg, const RptPageObject &obj);

#endif // RPTPAGEOBJECT_H
