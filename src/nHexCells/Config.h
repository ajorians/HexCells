#ifndef CONFIG_H
#define CONFIG_H

extern "C"
{
#ifdef _TINSPIRE
#include <os.h>
#include "ArchiveLib/ArchiveLib.h"
#endif
}

#define MAKE_CONFIG_ITEM(type, name) \
public: \
   type Get##name() const { return m_##name; }\
   void Set##name(type val) { m_##name = val; }\
protected: \
   type m_##name;

class Config
{
public:
   Config();
   ~Config();

   //MAKE_CONFIG_ITEM(int, ScoreLimit);

   //Achievements
   //MAKE_CONFIG_ITEM(int, GamesPlayed);

public:

protected:
   //ArchiveLib	m_Archive;
};

#endif

