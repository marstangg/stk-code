//  $Id: KartManager.cxx,v 1.7 2004/10/20 22:44:14 jamesgregory Exp $
//
//  TuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <set>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Loader.h"
#include "StringUtils.h"
#include "KartManager.h"

KartManager kart_manager;

KartManager::KartManager()
{
}

KartManager::~KartManager()
{
  for(KartPropertiesVector::iterator i = karts.begin(); i != karts.end(); ++i)
    delete *i;
}

int
KartManager::getKartId(const std::string ident)
{
  int j = 0;
  for(KartPropertiesVector::const_iterator i = karts.begin();
      i != karts.end(); ++i)
    {
      if ((*i)->ident == ident)
        return j;
      ++j;
    }

  throw std::runtime_error("KartManager: Couldn't find kart: '" + ident + "'");
}

const KartProperties*
KartManager::getKart(const std::string ident)
{
  for(KartPropertiesVector::const_iterator i = karts.begin();
      i != karts.end(); ++i)
    {
      if ((*i)->ident == ident)
        return *i;
    }

  throw std::runtime_error("KartManager: Couldn't find kart: '" + ident + "'");
}

const KartProperties*
KartManager::getKartById(int i)
{
  if (i < 0 || i >= int(karts.size()))
    throw std::runtime_error("KartManager: Couldn't find kart: '" + StringUtils::to_string(i) + "'");
    
  return karts[i];
}

void
KartManager::loadKartData()
{
  std::set<std::string> result;
  loader->listFiles(result, "data");

  // Findout which characters are available and load them
  for(std::set<std::string>::iterator i = result.begin(); i != result.end(); ++i)
    {
      if (StringUtils::has_suffix(*i, ".tkkf"))
        {
          karts.push_back(new KartProperties("data/" + *i));
        }
    }
}

std::vector<std::string>
KartManager::getRandomKarts(int len)
{
  std::vector<std::string> all_karts;

  for(KartPropertiesVector::const_iterator i = karts.begin();
      i != karts.end(); ++i)
    {
      all_karts.push_back((*i)->ident);
    }

  std::random_shuffle(all_karts.begin(), all_karts.end());

  all_karts.resize(len);

  return all_karts;
}

void
KartManager::fillWithRandomKarts(std::vector<std::string>& vec)
{
  std::vector<std::string> all_karts;

  for(KartPropertiesVector::const_iterator i = karts.begin();
      i != karts.end(); ++i)
    all_karts.push_back((*i)->ident);

  std::random_shuffle(all_karts.begin(), all_karts.end());

  int new_kart = 0;
  for(int i = 0; i < int(vec.size()); ++i)
    {
      while(vec[i].empty())
        {
          if (std::find(vec.begin(), vec.end(), all_karts[new_kart]) == vec.end())
            { // Found a new kart, so use it
              vec[i] = all_karts[new_kart];
            }
          else if (!all_karts.size() >= vec.size())
            { // We need to fill more karts than we have available, so don't care about dups
              vec[i] = all_karts[new_kart];
            }
          
          new_kart += 1;
        }
    }  
}

/* EOF */
