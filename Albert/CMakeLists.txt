cmake_minimum_required (VERSION 2.8.11)
project (Albert)

add_executable(db2src
    decomp.cpp
    tools/AlARMCode.cpp
    tools/EasyBMP/EasyBMP.cpp
    tools/AlData.cpp
    tools/AlSymbol.cpp
    tools/AlSymbolList.cpp
    tools/AlType.cpp
    tools/AlTypeList.cpp
    tools/AlTypeRef.cpp
    tools/AlObject.cpp
    tools/AlDatabaseObject.cpp
    tools/disarm_c.cpp
    tools/nsDecode.cpp
    tools/AlCMethod.cpp
    tools/AlCPPMethod.cpp
    tools/AlMemoryMap.cpp
    tools/AlDatabase.cpp
    tools/db2src.cpp
    tools/disarm.cpp)

target_include_directories(db2src PUBLIC tools tools/EasyBMP)

add_executable(sym2db
    decomp.cpp
    tools/disarm_c.cpp
    tools/disarm.cpp
    tools/AlARMCode.cpp
    tools/EasyBMP/EasyBMP.cpp
    tools/AlData.cpp
    tools/AlSymbol.cpp
    tools/AlSymbolList.cpp
    tools/AlType.cpp
    tools/AlTypeList.cpp
    tools/AlTypeRef.cpp
    tools/AlObject.cpp
    tools/AlDatabaseObject.cpp
    tools/nsDecode.cpp
    tools/AlCMethod.cpp
    tools/AlCPPMethod.cpp
    tools/AlMemoryMap.cpp
    tools/AlDatabase.cpp
    tools/sym2db.cpp
    tools/disarm.cpp)

target_include_directories(sym2db PUBLIC tools tools/EasyBMP)
