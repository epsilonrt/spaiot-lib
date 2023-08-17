/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#include "serversettings_p.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                ServerSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation
  ServerSettings::Private::Private () : publishInterval (30000)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  ServerSettings::ServerSettings (Private &dd) : d_ptr (&dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  ServerSettings::ServerSettings () : ServerSettings (*new Private)
  {}

  //----------------------------------------------------------------------------
  ServerSettings::ServerSettings (const String &model, unsigned long interval) :
    ServerSettings (*new Private) {
    PIMPL_D (ServerSettings);

    d->publishInterval = interval;
    d->spaModel = model;
  }

  //----------------------------------------------------------------------------
  ServerSettings::ServerSettings (const char *model, unsigned long interval) :
    ServerSettings (*new Private) {
    PIMPL_D (ServerSettings);

    d->publishInterval = interval;
    d->spaModel = model;
  }

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  ServerSettings::~ServerSettings() {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  ServerSettings::ServerSettings (const ServerSettings &other) :
    ServerSettings (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  ServerSettings &ServerSettings::operator= (const ServerSettings &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  ServerSettings::ServerSettings (ServerSettings &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  ServerSettings &ServerSettings::operator= (ServerSettings &&other) = default;

  //----------------------------------------------------------------------------
  bool ServerSettings::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool ServerSettings::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const ServerSettings);

      return d->spaModel.isEmpty() && d->publishInterval == 30000;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void ServerSettings::clear() {

    if (isNull()) {

      d_ptr.reset (new Private);
    }
    else {

      d_ptr->publishInterval = 30000;
      d_ptr->spaModel.clear();
    }
  }

  //----------------------------------------------------------------------------
  const String &ServerSettings::spaModel() const {
    PIMPL_D (const ServerSettings);

    return d->spaModel;
  }

  //----------------------------------------------------------------------------
  unsigned long ServerSettings::publishInterval() const {
    PIMPL_D (const ServerSettings);

    return d->publishInterval;
  }

  //----------------------------------------------------------------------------
  void ServerSettings::setSpaModel (const String &spaModel) {
    PIMPL_D (ServerSettings);

    d->spaModel = spaModel;
  }

  //----------------------------------------------------------------------------
  void ServerSettings::setPublishInterval (unsigned long interval) {
    PIMPL_D (ServerSettings);

    d->publishInterval = interval;
  }

  //----------------------------------------------------------------------------
  bool ServerSettings::operator== (const ServerSettings &other) const {
    PIMPL_D (const ServerSettings);

    return d->publishInterval == other.d_func()->publishInterval &&
           d->spaModel == other.d_func()->spaModel;
  }

  //----------------------------------------------------------------------------
  bool ServerSettings::operator!= (const ServerSettings &other) const {
    PIMPL_D (const ServerSettings);

    return ! (*this == other);
  }

}
//------------------------------------------------------------------------------
