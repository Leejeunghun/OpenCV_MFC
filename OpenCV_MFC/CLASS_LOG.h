#pragma once
#include "OpenCV_MFCDlg.h"

class CLASS_LOG
{
public:
	CLASS_LOG(void);
	int create_log_file();
	int write_log_file(char* str, ...);
	void addLog(CString msg);
	CString logpath;

};

