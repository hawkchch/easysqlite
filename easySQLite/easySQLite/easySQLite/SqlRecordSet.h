//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include <vector>
#include "sqlite3.h"
#include "SqlCommon.h"
#include "SqlDatabase.h"
#include "SqlRecord.h"


namespace sql
{

class RecordSet
{
private:
	enum
	{
		DATASET_ITERATION_CONTINUE = 0,
		DATASET_ITERATION_ABORT = 1,
	};

private:
  Database *_db;
	sqlite3* _handle;
	string _err_msg;
	int _result_query;
	FieldSet _fields;
	std::vector<Record> _records;

private:
	static int on_next_record(void* param, int column_count, char** values, char** columns);

public:
  RecordSet(Database &db);
  RecordSet(Database &db, Field* definition);
  RecordSet(Database &db, FieldSet* fields);

  RecordSet(sqlite3* db);
	RecordSet(sqlite3* db, Field* definition);
	RecordSet(sqlite3* db, FieldSet* fields);
	~RecordSet(void);

public:
	string errMsg();
	bool isResult();
	bool query(string sql);
	void close();
	FieldSet* fields();

public:
	int count();
	Record* getRecord(int record_index);
	Record* getTopRecord();
	Value* getTopRecordFirstValue();
	string toString();

};


//sql eof
};
