#ifndef TCPCOMMAND_LOGGEDINUSER_ISINSYSTEMGROUPFACTORY_H
#define TCPCOMMAND_LOGGEDINUSER_ISINSYSTEMGROUPFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommand_loggedInUser_isInSystemGroupFactory : public TcpAbstractCommandFactory
{
public:
    TcpCommand_loggedInUser_isInSystemGroupFactory();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMAND_LOGGEDINUSER_%actionName:uFACTORY%_H

