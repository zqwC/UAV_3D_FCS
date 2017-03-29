#ifndef FILE_SQL_OPERATION_H
#define FILE_SQL_OPERATION_H

#include "../stdafx.h"


class SQLOperation
{
public:
	SQLOperation();
	~SQLOperation();

	//连接打开数据库
	BOOL OpenDataBase(CString strConnection, 
							   CString strUserID, CString strPassword);
	//断开关闭数据库
	BOOL CloseDataBase();

	//执行命令函数
	BOOL Execute(const CString& strCommand);

	//查询函数
	BOOL Select(const CString& strCommand);

	//更新数据
	BOOL Update(const CString& table, const CString& col,
		const CString& data, const CString& key, const CString& keyValue);
    //ACCOUNT_INFOR m_AccountInfor;

	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordSet;
	_CommandPtr m_pCommand;
};
#endif