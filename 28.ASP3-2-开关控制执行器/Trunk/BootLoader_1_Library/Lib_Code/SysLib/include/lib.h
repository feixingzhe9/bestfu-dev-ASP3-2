#ifndef LIB_H
#define LIB_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "thread.h"
#include "DataEncrypt.h"
#include "crc.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "FifoDataProc.h"
#include "FaultManage.h"
#include "PhyMsgProc.h"
#include "UserData.h"
#include "Updata.h"
#include "UnitCfg.h"
#include "DeviceIDSetAttr.h"
#include "DeviceUpgradeAttr.h"	
#include "CMDProcess.h"
#include "Object.h"
#include "MsgPackage.h"
#include "DevPartTab.h"

/*
------------------------------------------------------------------------------------------------------------------------
*                                                      ERROR CODES
------------------------------------------------------------------------------------------------------------------------
*/

#define  ASSERT(CON)        if (!(CON)) {volatile CPU_INT08U dummy = 0; CPU_SR_Save(); while (dummy == 0);}

#ifdef __cplusplus
}
#endif

#endif
