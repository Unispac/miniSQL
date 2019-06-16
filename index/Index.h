#ifndef Index_H
#define Index_H

#include <string>
using namespace std;

class Index
{
public:
	Index(const char* data);

	const char* getName() const;

	const char* getTableName() const;

	const char* getColName() const;

private:
	string name;

	string tableName;

	string colName;
};

#endif // !Index_H
