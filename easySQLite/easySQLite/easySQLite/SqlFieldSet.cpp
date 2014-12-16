#include "SqlFieldSet.h"


namespace sql
{

FieldSet::FieldSet(const Field* definition)
{
	//make fields map from array
	if (definition)
	{
		int index = 0;
		while (true)
		{
			const Field& field = definition[index];

			if (field.isEndingField())
				break;
      
      Field tempField = field;

			tempField._index = index;

			_vec.push_back(tempField);

			index++;
		}

    //make fields map from vector
    for (int index = 0; index < (int)_vec.size(); index++)
    {
      Field& field = _vec[index];
      _map[field.getName()] = &field;
    }
	}
}

FieldSet::FieldSet(std::vector<Field>& definition)
{
	copy(definition);
}

FieldSet::FieldSet(const FieldSet& source)
{
	copy(source._vec);
}

void FieldSet::copy(const std::vector<Field>& definition)
{
	_vec = definition;

	//make fields map from vector
	for (int index = 0; index < (int)_vec.size(); index++)
	{
		Field& field = _vec[index];
		_map[field.getName()] = &field;
	}
}

int FieldSet::count() const
{
	return (int)_vec.size();
}

const Field* FieldSet::getByIndex(int index) const
{
  FieldSet *pThis = const_cast<FieldSet *>(this);

  if ((index >= 0) && (index < count()))
    return &(pThis->_vec[index]);

	return NULL;
}

const Field* FieldSet::getByName(string name) const
{
  FieldSet *pThis = const_cast<FieldSet *>(this);
	return pThis->_map[name];
}

string FieldSet::getDefinition(bool stripIgnoredFields) const
{
	string s;

	for (int index = 0; index < count(); index++)
	{
		if (const Field* f = getByIndex(index))
		{
      if ((f->isIgnored()) &&
          (stripIgnoredFields)) continue;

      if (!s.empty()) s += ", ";
			s += f->getDefinition();
		}
	}

	return s;
}

bool FieldSet::hasIgnoredFields() const
{
  for (int index = 0; index < count(); index++)
  {
    if (const Field* f = getByIndex(index))
    {
      if (f->isIgnored()) return true;
    }
  }
  
  return false;
}

string FieldSet::definitionHash(bool stripIgnoredFields) const
{
	return generateSHA(getDefinition(stripIgnoredFields));
}

string FieldSet::toString()
{
	string s;

	for (int index = 0; index < count(); index++)
	{
		if (const Field* f = getByIndex(index))
		{
      if (f->isIgnored()) continue;

      if (!s.empty()) s += ", ";
			s += f->getName();
		}
	}

	return s;
}

FieldSet* FieldSet::createFromDefinition(string value)
{
	std::vector<string> vec;
	listToVector(value, vec, ",");

	std::vector<Field> fields;

	for (int index = 0; index < (int)vec.size(); index++)
	{
		std::string definition = vec[index];

		if (Field* field = Field::createFromDefinition(definition))
		{
			Field f(*field);
			fields.push_back(f);
			delete field;
		} else {
			return NULL;
		}
	}

	return new FieldSet(fields);
}


//sql eof
};
