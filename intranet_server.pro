QT += network sql

include(Logger/Logger.pri)
include(MyLdap/MyLdap.pri)

LIBS += -lldap -llber

SOURCES += \
    main.cpp \
    TcpServer.cpp \
    TcpSocketThread.cpp \
    TcpAbstractCommand.cpp \
    TcpCommands/main/userLogin.cpp \
    TcpCommands/main/serverVersion.cpp \
    TcpCommands/loggedInUser/getUserColumn.cpp \
    TcpCommands/loggedInUser/getUserColumnFactory.cpp \
    TcpCommandFactories.cpp \
    TcpAbstractCommandFactory.cpp \
    TcpCommands/main/userLoginFactory.cpp \
    TcpCommands/main/serverVersionFactory.cpp \
    TcpCommandsHelper.cpp \
    TcpCommands/main/connectTcp.cpp \
    TcpCommands/main/connectTcpFactory.cpp \
    dbModels/dbAbstractModel.cpp \
    dbModels/dbUserModel.cpp \
    Database/Database.cpp \
    Database/DatabaseQuery.cpp \
    Config.cpp \
    dbModels/dbTcpActionModel.cpp \
    TcpCommands/loggedInUser/isInSystemGroupFactory.cpp \
    TcpCommands/loggedInUser/isInSystemGroup.cpp \
    dbModels/dbSystemGroupsModel.cpp \
    TcpCommands/system/getAllTcpCommandsFactory.cpp \
    TcpCommands/system/getAllTcpCommands.cpp \
    TcpCommands/system/getAllSystemGroupsFactory.cpp \
    TcpCommands/system/getAllSystemGroups.cpp \
    TcpCommands/system/saveTcpCommandsPermissionsFactory.cpp \
    TcpCommands/system/saveTcpCommandsPermissions.cpp \
    TcpCommands/administration/testLDAPGroupMemberships.cpp \
    TcpCommands/administration/testLDAPGroupMembershipsFactory.cpp \
    TcpCommands/administration/userGroupModFactory.cpp \
    TcpCommands/administration/userGroupMod.cpp \
    dbModels/dbUserGroupsModel.cpp \
    TcpCommands/administration/getAllUserGroupsFactory.cpp \
    TcpCommands/administration/getAllUserGroups.cpp \
    TcpCommands/administration/getUserGroupFactory.cpp \
    TcpCommands/administration/getUserGroup.cpp \
    TcpCommands/administration/userGroupDeleteFactory.cpp \
    TcpCommands/administration/userGroupDelete.cpp \
    TcpCommands/administration/userGroupToSystemGroupsFactory.cpp \
    TcpCommands/administration/userGroupToSystemGroups.cpp \
    TcpCommands/backupMonitor/getCustomersFactory.cpp \
    TcpCommands/backupMonitor/getCustomers.cpp \
    TcpCommands/customers/searchFactory.cpp \
    TcpCommands/customers/search.cpp \
    TcpCommands/customers/getCustomerFactory.cpp \
    TcpCommands/customers/getCustomer.cpp \
    TcpCommands/customers/modCustomerFactory.cpp \
    TcpCommands/customers/modCustomer.cpp \
    TcpCommands/customers/generateNewBackupMonitorKeyFactory.cpp \
    TcpCommands/customers/generateNewBackupMonitorKey.cpp \
    TcpCommands/backupMonitor/getJobsFactory.cpp \
    TcpCommands/backupMonitor/getJobs.cpp \
    TcpCommands/backupMonitor/getJobFactory.cpp \
    TcpCommands/backupMonitor/getJob.cpp \
    TcpCommands/backupMonitor/getJobProtocolFactory.cpp \
    TcpCommands/backupMonitor/getJobProtocol.cpp \
    Database/DatabasePool.cpp \
    Utils/UtilDataStream.cpp \
    Utils/UtilUsers.cpp \
    TcpCommands/system/serverStatisticsFactory.cpp \
    TcpCommands/system/serverStatistics.cpp \
    TcpCommands/loggedInUser/hasTcpPermissionFactory.cpp \
    TcpCommands/loggedInUser/hasTcpPermission.cpp \
    Registry.cpp \
    TcpCommands/backupMonitor/getBackupStateOfDayFactory.cpp \
    TcpCommands/backupMonitor/getBackupStateOfDay.cpp \
    Utils/UtilBackupMonitor.cpp \
    TcpCommands/notifications/getLastMessagesFactory.cpp \
    TcpCommands/notifications/getLastMessages.cpp \
    TcpCommands/notifications/createFactory.cpp \
    TcpCommands/notifications/create.cpp \
    TcpCommands/backupMonitor/getCustomerGroupsFactory.cpp \
    TcpCommands/backupMonitor/getCustomerGroups.cpp \
    TcpCommands/backupMonitor/customerGroupModFactory.cpp \
    TcpCommands/backupMonitor/customerGroupMod.cpp \
    TcpCommands/backupMonitor/customerGroupDeleteFactory.cpp \
    TcpCommands/backupMonitor/customerGroupDelete.cpp \
    TcpCommands/customers/customerEditableFactory.cpp \
    TcpCommands/customers/customerEditable.cpp \
    TcpCommands/customers/removeCustomerFactory.cpp \
    TcpCommands/customers/removeCustomer.cpp \
    TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.cpp \
    TcpCommands/backupMonitor/getCustomerInterfaceAddress.cpp \
    TcpCommands/backupMonitor/getLog.cpp \
    TcpCommands/backupMonitor/getLogFactory.cpp \
    TcpCommands/backupMonitor/createLog.cpp \
    TcpCommands/backupMonitor/createLogFactory.cpp \
    BackupMonitorDoCacheThread.cpp

HEADERS += \
    TcpServer.h \
    TcpSocketThread.h \
    TcpAbstractCommand.h \
    TcpCommands/main/userLogin.h \
    TcpCommands/main/serverVersion.h \
    TcpCommandFactories.h \
    TcpAbstractCommandFactory.h \
    TcpCommands/main/serverVersionFactory.h \
    TcpCommands/main/userLoginFactory.h \
    TcpCommands/main/connectTcp.h \
    TcpCommands/main/connectTcpFactory.h \
    TcpCommands/loggedInUser/getUserColumn.h \
    TcpCommands/loggedInUser/getUserColumnFactory.h \
    dbModels/dbAbstractModel.h \
    dbModels/dbUserModel.h \
    Database/Database.h \
    Database/DatabaseQuery.h \
    globals.h \
    Config.h \
    TcpCommandsHelper.h \
    dbModels/dbTcpActionModel.h \
    TcpCommands/loggedInUser/isInSystemGroupFactory.h \
    TcpCommands/loggedInUser/isInSystemGroup.h \
    dbModels/dbSystemGroupsModel.h \
    TcpCommands/system/getAllTcpCommandsFactory.h \
    TcpCommands/system/getAllTcpCommands.h \
    TcpCommands/system/getAllSystemGroupsFactory.h \
    TcpCommands/system/getAllSystemGroups.h \
    TcpCommands/system/saveTcpCommandsPermissionsFactory.h \
    TcpCommands/system/saveTcpCommandsPermissions.h \
    TcpCommands/administration/testLDAPGroupMemberships.h \
    TcpCommands/administration/testLDAPGroupMembershipsFactory.h \
    TcpCommands/administration/userGroupModFactory.h \
    TcpCommands/administration/userGroupMod.h \
    dbModels/dbUserGroupsModel.h \
    TcpCommands/administration/getAllUserGroupsFactory.h \
    TcpCommands/administration/getAllUserGroups.h \
    Database/DatabaseTypeDefs.h \
    TcpCommands/administration/getUserGroupFactory.h \
    TcpCommands/administration/getUserGroup.h \
    TcpCommands/administration/userGroupDeleteFactory.h \
    TcpCommands/administration/userGroupDelete.h \
    TcpCommands/administration/userGroupToSystemGroupsFactory.h \
    TcpCommands/administration/userGroupToSystemGroups.h \
    TcpCommands/backupMonitor/getCustomersFactory.h \
    TcpCommands/backupMonitor/getCustomers.h \
    TcpCommands/customers/searchFactory.h \
    TcpCommands/customers/search.h \
    TcpCommands/customers/getCustomerFactory.h \
    TcpCommands/customers/getCustomer.h \
    TcpCommands/customers/modCustomerFactory.h \
    TcpCommands/customers/modCustomer.h \
    TcpCommands/customers/generateNewBackupMonitorKeyFactory.h \
    TcpCommands/customers/generateNewBackupMonitorKey.h \
    TcpCommands/backupMonitor/getJobsFactory.h \
    TcpCommands/backupMonitor/getJobs.h \
    TcpCommands/backupMonitor/getJobFactory.h \
    TcpCommands/backupMonitor/getJob.h \
    TcpCommands/backupMonitor/getJobProtocolFactory.h \
    TcpCommands/backupMonitor/getJobProtocol.h \
    Database/DatabasePool.h \
    Utils/UtilDataStream.h \
    Utils/UtilUsers.h \
    TcpCommands/system/serverStatisticsFactory.h \
    TcpCommands/system/serverStatistics.h \
    TcpCommands/loggedInUser/hasTcpPermissionFactory.h \
    TcpCommands/loggedInUser/hasTcpPermission.h \
    Registry.h \
    TcpCommands/backupMonitor/getBackupStateOfDayFactory.h \
    TcpCommands/backupMonitor/getBackupStateOfDay.h \
    Utils/UtilBackupMonitor.h \
    TcpCommands/notifications/getLastMessagesFactory.h \
    TcpCommands/notifications/getLastMessages.h \
    TcpCommands/notifications/createFactory.h \
    TcpCommands/notifications/create.h \
    TcpCommands/backupMonitor/getCustomerGroupsFactory.h \
    TcpCommands/backupMonitor/getCustomerGroups.h \
    TcpCommands/backupMonitor/customerGroupModFactory.h \
    TcpCommands/backupMonitor/customerGroupMod.h \
    TcpCommands/backupMonitor/customerGroupDeleteFactory.h \
    TcpCommands/backupMonitor/customerGroupDelete.h \
    TcpCommands/customers/customerEditableFactory.h \
    TcpCommands/customers/customerEditable.h \
    TcpCommands/customers/removeCustomerFactory.h \
    TcpCommands/customers/removeCustomer.h \
    TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.h \
    TcpCommands/backupMonitor/getCustomerInterfaceAddress.h \
    TcpCommands/backupMonitor/getLog.h \
    TcpCommands/backupMonitor/getLogFactory.h \
    TcpCommands/backupMonitor/createLog.h \
    TcpCommands/backupMonitor/createLogFactory.h \
    BackupMonitorDoCacheThread.h

OTHER_FILES +=
























































































































































































win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/ -lldap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/ -lldap
else:symbian: LIBS += -lldap
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lldap

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
