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
#include "event_p.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                  Event
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Event::Private::Private (Type type, uint16_t value) : type (type), value (value)
  {}

  //----------------------------------------------------------------------------
  Event::Private::Private() : Private (Type::NoEvent, 0) {}

  //----------------------------------------------------------------------------
  // Map to convert Event::Type to string
  const std::map <Event::Type, String> Event::Private::TypeToString = {
    {Event::Type::PowerOn, "PowerOn"},
    {Event::Type::FilterOn, "FilterOn"},
    {Event::Type::BubbleOn, "BubbleOn"},
    {Event::Type::JetOn, "JetOn"},
    {Event::Type::SanitizerOn, "SanitizerOn"},
    {Event::Type::HeaterOn, "HeaterOn"},
    {Event::Type::HeatReached, "PumHeatReachedp2"},
    {Event::Type::WaterTemp, "WaterTemp"},
    {Event::Type::DesiredTemp, "DesiredTemp"},
    {Event::Type::SanitizerTime, "SanitizerTime"},
    {Event::Type::ErrorCode, "ErrorCode"},
    {Event::Type::AmbientTemperature, "AmbientTemperature"},
    {Event::Type::NoEvent, "NoEvent"},
    {Event::Type::AnyEvent, "AnyEvent"}
  };

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Event::Event (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Event::Event (Type type, uint16_t value) :
    Event (*new Private (type, value)) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  Event::~Event() {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  Event::Event (const Event &other) :
    Event (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Event &Event::operator= (const Event &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  Event::Event (Event &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  Event &Event::operator= (Event &&other) = default;

  //----------------------------------------------------------------------------
  bool Event::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool Event::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const Event);

      return d->value == 0 && d->type == Type::NoEvent;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Event::clear() {

    if (isNull()) {

      d_ptr.reset (new Private);
    }
    else {

      d_ptr->value = 0;
      d_ptr->type = Type::NoEvent;
    }
  }

  //----------------------------------------------------------------------------
  void Event::setValue (uint16_t value) {
    PIMPL_D (Event);

    d->value = value;
  }

  //----------------------------------------------------------------------------
  void Event::setType (Type type) {
    PIMPL_D (Event);

    d->type = type;
  }

  //----------------------------------------------------------------------------
  Event::Type Event::type() const {
    PIMPL_D (const Event);

    return d->type;
  }

  //----------------------------------------------------------------------------
  uint16_t Event::value() const {
    PIMPL_D (const Event);

    return d->value;
  }

  //----------------------------------------------------------------------------
  Event::operator bool() const {
    PIMPL_D (const Event);

    return (d->type != Type::NoEvent) && (d->value == true);
  }

  //----------------------------------------------------------------------------
  Event::operator uint16_t() const {
    PIMPL_D (const Event);

    return (d->type != Type::NoEvent) ? d->value : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  bool Event::operator == (const Event &other) const {
    PIMPL_D (const Event);

    return (d->type == other.d_func()->type) && (d->value == other.d_func()->value);
  }

  //----------------------------------------------------------------------------
  bool Event::operator != (const Event &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  String Event::toString() const {
    PIMPL_D (const Event);

    return Private::TypeToString.at (d->type) + ", value: " + String (d->value);
  }

  //----------------------------------------------------------------------------
  String Event::typeToString (Type type) {

    return Private::TypeToString.at (type);
  }
}
//------------------------------------------------------------------------------
