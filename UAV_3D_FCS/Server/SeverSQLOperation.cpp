#include "stdafx.h"
#include"..\stdafx.h"
#include "ServerSQLOperation.h"


SQLOperation::SQLOperation()
{
}

SQLOperation::~SQLOperation()
{
}	

//在数据库中查询集合 //////////////////////////////////// 
BOOL SQLOperation::Select(const CString& strCommand)
{
	HRESULT hr;
	_bstr_t bstrCommand(strCommand);
	//创建Recordset对象
	hr = m_pRecordSet.CreateInstance(TEXT("ADODB.Recordset"));
	m_pRecordSet->CursorLocation  =  adUseClient;	
	if (!SUCCEEDED(hr))
	{
		AfxMessageBox(TEXT("创建Recordser对象失败"));
		return FALSE;
	}
	try
	{
		m_pRecordSet->Open(bstrCommand, m_pConnection.GetInterfacePtr(), 
			adOpenStatic, adLockOptimistic, adCmdText);
	}
	catch(_com_error err)
	{
		AfxMessageBox((LPCTSTR)err.Description());
		return FALSE;
	}
	//m_pRecordSet.Release();
	return  TRUE;
}
////////////////////////////////////////////////////////////////////

//执行特定语句 ///////////////////////////////////////////////////
BOOL SQLOperation::Execute(const CString& strCommand)
{
	HRESULT hr;
	try
	{
		hr = m_pCommand.CreateInstance(TEXT("ADODB.Command"));
		if (!SUCCEEDED(hr))
		{
			AfxMessageBox(TEXT("命令创建失败"));
			return FALSE;
		}
	}
	catch(_com_error err)
	{
		AfxMessageBox(err.Description());
		return FALSE;
	}

	m_pCommand->put_ActiveConnection(_variant_t((IDispatch*)m_pConnection, TRUE));
	m_pCommand->CommandText = _bstr_t(strCommand);
	try
	{
		m_pCommand->Execute(NULL, NULL, adCmdText);
	}
	catch(_com_error ce)
	{
		CString error;
		error.Format(TEXT("错误消息为:%s\r\n"), (LPCTSTR)ce.Description());
		AfxMessageBox(error);
		return FALSE;
	}
	//m_pCommand.Release();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////

BOOL SQLOperation::Update(const CString& table, const CString& col, 
		const CString& data, const CString& key, const CString& keyValue)
{
	CString Command = TEXT("");
	Command = TEXT("UPDATE ") + table + TEXT(" SET ") + col + TEXT(" = ") + data + 
		TEXT(" WHERE ") + key + TEXT(" = ") + keyValue;
	Execute(Command);
	return TRUE;
}

BOOL SQLOperation::OpenDataBase(CString strConnection, 
							   CString strUserID, CString strPassword)
{
	if (m_pConnection && m_pConnection->GetState() == adStateOpen)
	{
		//AfxMessageBox(TEXT("数据库已连接"));
		return TRUE;
	}
	HRESULT hr;
	hr = ::CoInitialize(NULL);	//初始化COM

	if (!SUCCEEDED(hr))
	{
		return FALSE;
	}
	//创建Connection对象
	hr = m_pConnection.CreateInstance(TEXT("ADODB.Connection"));
	if (!SUCCEEDED(hr))
	{
		return FALSE;
	}
	_bstr_t bstrConnect(strConnection);
	_bstr_t bstrUserID(strUserID);
	_bstr_t bstrPassword(strPassword);

	try
	{
		//连接数据库
		hr = m_pConnection->Open(bstrConnect, bstrUserID, bstrPassword, adModeUnknown);	
		if (!SUCCEEDED(hr))
		{
			return FALSE;
		}
	}
	catch(_com_error err)
	{
		CString strError;                     
		strError.Format(TEXT("连接数据库失败信息!\r\n错误信息:%s"), (LPCTSTR)err.Description());  
		AfxMessageBox(strError);     
		return FALSE;
	}

	//提示用户连接数据库成功
	AfxMessageBox(TEXT("连接数据库成功")); 
	return TRUE;
}

BOOL SQLOperation::CloseDataBase()
{
	if (m_pConnection == NULL)
	{
		return TRUE;
	}
	try
	{
		m_pConnection->Close();
		m_pConnection = NULL;
	}
	catch(_com_error err)
	{
		CString strError;                     
		strError.Format( err.Description() );  
		AfxMessageBox(strError);  
		TRACE("关闭数据库发生错误:%s\n", err.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}