#include <QtGui/QApplication>
#include "messagetransceiver.h"
#include "protocolhandler.h"
#include "collaborationserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MessageTransceiver m;
    ProtocolHandler p;
    CollaborationServer cs;

    p.setMessageTransceiver(&m);
    cs.setProtocolHandler(&p);

    m.run();

    return a.exec();
}
