#include "pch.h"
#include "CLASS_LOG.h"
#include "pch.h"


CLASS_LOG::CLASS_LOG(void)
{

}

int CLASS_LOG::create_log_file()
{
	TCHAR chFilePath[256] = { 0, };
	GetModuleFileName(NULL, chFilePath, 256);

	CString strlogPath(chFilePath);
	strlogPath = strlogPath.Left(strlogPath.ReverseFind('\\'));

	CStdioFile logfile;
	CString fname = NULL;

	SYSTEMTIME tm;
	GetLocalTime(&tm);
	fname.Format("%s\\Log_%04d-%02d-%02d.txt", strlogPath, tm.wYear, tm.wMonth, tm.wDay);

	logpath.Format(fname);

	if (logfile.Open(logpath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::typeText) == FALSE)
	{
		return(1);
	}

	logfile.SeekToEnd();

	CStringA header("\r\n\r\n------------------------------ Log File Start------------------------------\r\n");

	logfile.Write(header, header.GetLength());
	logfile.Close();

	return(0);
}

int CLASS_LOG::write_log_file(char* str, ...)
{
	CStdioFile logfile;
	SYSTEMTIME lpSystemTime;
	if (logfile.Open(logpath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::typeText) == FALSE)
	{
		return(1);
	}

	logfile.SeekToEnd();

	char buff[1024];
	va_list ap;
	va_start(ap, str);
	vsprintf(buff, str, ap);
	va_end(ap);


	GetLocalTime(&lpSystemTime); // 현재 날짜, 시간 가져오는 함수
	CStringA l_str;
	l_str.Format("[%02d:%02d:%02d] ", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds);

	logfile.Write(l_str, l_str.GetLength());
	logfile.Write(buff, strlen(buff));
	logfile.Write((const void*)"\r\n", 2);
	logfile.Close();

	CString _str1, _str2;
	_str1.Format(_T("%s"), l_str);
	_str2.Format(_T("%s"), str);
	addLog(_str1);
	addLog(_str2);
	return(0);
}

void CLASS_LOG::addLog(CString msg)
{
	COpenCVMFCDlg* pMainDlg = (COpenCVMFCDlg*)AfxGetMainWnd();

	pMainDlg->m_log += _T(msg);
	//** Edit Control에 개행
	pMainDlg->m_log += _T("\r\n");
	//** Edit Control에 대한 업데이트 진행
	pMainDlg->UpdateData(FALSE);

	pMainDlg->m_ed_log.LineScroll(pMainDlg->m_ed_log.GetLineCount());      //자동 스크롤
}