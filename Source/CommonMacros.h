#pragma once

#define VALID_CHECK(classname, valname, doing, returnVal)	\
	classname* valname = doing;								\
	if (!IsValid(valname))									\
		return returnVal;

#define CAST_VALID_CHECK(classname, valname, param, returnVal)	\
	classname* valname = Cast<classname>(param);				\
	if (!IsValid(valname))										\
		return returnVal;


#define CREATE_VALID_CHECK(classname, valname, objname, returnVal)	\
	valname = CreateDefaultSubobject<classname>(objname);			\
	if (!IsValid(valname))											\
		return returnVal;


#define GETCOMP_VALID_CHECK(classname, valname, returnVal)	\
	valname = GetComponentByClass<classname>();				\
	if (!IsValid(valname))									\
		return returnVal;

