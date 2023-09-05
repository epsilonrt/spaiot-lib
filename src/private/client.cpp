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
#include "client_p.h"
#include "unittest_p.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            SpaClient
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  SpaClient::Private::Private (const char *className, SpaClient *q) :
    className (className),
    q_ptr (q),
    isopen (false) {

    const std::set <Event::Type> states = {
      Event::Type::PowerOn,
      Event::Type::FilterOn,
      Event::Type::BubbleOn,
      Event::Type::HeaterOn,
      Event::Type::HeatReached,
      Event::Type::JetOn,
      Event::Type::SanitizerOn
    };
    for (auto s : states) {
      spaValues[s] = UnsetValue8;
    }

    const std::set <Event::Type> values = {
      Event::Type::WaterTemp,
      Event::Type::DesiredTemp,
      Event::Type::SanitizerTime
    };
    for (auto v : values) {
      spaValues[v] = UnsetValue16;
    }
    spaValues[Event::Type::ErrorCode] = 0;
  }

  //----------------------------------------------------------------------------
  SpaClient::Private::Private (const char *className, const std::set<Event::Type> &sEvents, SpaClient *q) :
    Private (className, q) {

    subscribedEvents = sEvents;
  }
  //----------------------------------------------------------------------------
  void SpaClient::Private::pushToSpa (const Event &event) {

    if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {

      if (subscribedEvents.empty() || subscribedEvents.count (event.type()) > 0) {

        outQ.push (event);
      }
    }
  }

  //----------------------------------------------------------------------------
  bool SpaClient::Private::pullFromSpa (Event &event) {

    if (! inQ.empty()) {

      event = inQ.front();
      inQ.pop();
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool SpaClient::Private::pollSpaEvents () {
    Event event;

    while (pullFromSpa (event)) {

      TEST_PRINTF ("%s:%d: Event received %s, time: %d",
                   __PRETTY_FUNCTION__, __LINE__, event.toString().c_str(), millis());

      if (spaValues.count (event.type()) > 0) {

        spaValues[event.type()] = event.value();
      }
      else {

        event.setType (Event::NoEvent);
      }
    }
    return event.type() != Event::Type::NoEvent;
  }

  //----------------------------------------------------------------------------
  uint16_t SpaClient::Private::spaValue (Event::Type type) const {

    if (spaValues.count (type) > 0) {

      return spaValues.at(type);
    }
    return UnsetValue16;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  SpaClient::SpaClient (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  SpaClient::SpaClient (const char *className) :
    SpaClient (*new Private (className)) {}

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  SpaClient::SpaClient() :
    SpaClient (*new Private ("SpaClient")) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  SpaClient::~SpaClient() {}

  //----------------------------------------------------------------------------
  SpaClient::SpaClient (std::initializer_list<Event::Type> subscribedEvents) :
    SpaClient (*new Private ("SpaClient", std::set<Event::Type> (subscribedEvents)))
  {}

  //----------------------------------------------------------------------------
  SpaClient::SpaClient (const std::set<Event::Type> &subscribedEvents) :
    SpaClient (*new Private ("SpaClient", subscribedEvents))
  {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  SpaClient::SpaClient (const SpaClient &other) :
    SpaClient (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  SpaClient &SpaClient::operator= (const SpaClient &other) {

    if (this != &other) {
      PIMPL_D (SpaClient);

      d->outQ = other.d_func()->outQ;
      d->inQ = other.d_func()->inQ;
      d->subscribedEvents = other.d_func()->subscribedEvents;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  SpaClient::SpaClient (SpaClient &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  SpaClient &SpaClient::operator= (SpaClient &&other) = default;

  //----------------------------------------------------------------------------
  bool SpaClient::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool SpaClient::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const SpaClient);

      return d->outQ.empty() && d->inQ.empty() && d->subscribedEvents.empty();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void SpaClient::clear() {

    if (isNull()) {

      d_ptr.reset (new Private ("SpaClient"));
    }
    else {

      std::queue<Event>().swap (d_ptr->outQ); // Clear the queue, std::queue::clear() is not available in C++11
      std::queue<Event>().swap (d_ptr->inQ); // Clear the queue
      d_ptr->subscribedEvents.clear();
    }
  }

  //----------------------------------------------------------------------------
  bool SpaClient::operator== (const SpaClient &other) const {
    PIMPL_D (const SpaClient);

    return d->outQ == other.d_func()->outQ &&
           d->inQ == other.d_func()->inQ &&
           d->subscribedEvents == other.d_func()->subscribedEvents &&
           d->className == other.d_func()->className; // TODO: compare the class name ?
  }

  //----------------------------------------------------------------------------
  bool SpaClient::operator!= (const SpaClient &other) const {
    PIMPL_D (const SpaClient);

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  bool SpaClient::write (const Event &event) {

    if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {
      PIMPL_D (SpaClient);

      if (d->subscribedEvents.empty() || d->subscribedEvents.count (event.type()) > 0) {

        d->inQ.push (event);
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------------------------
  Event SpaClient::read () {
    PIMPL_D (SpaClient);
    Event event;

    if (available()) {

      event = d->outQ.front();
      d->outQ.pop();
    }
    return event;
  }

  //----------------------------------------------------------------------------
  Event SpaClient::read (Event::Type type) {
    PIMPL_D (SpaClient);
    Event event;

    if (available()) {

      event = d->outQ.front();
      if (event.type() == type || type == Event::AnyEvent) {

        d->outQ.pop();
      }
      else {

        event.setType (Event::NoEvent);
      }
    }
    return event;
  }

  //----------------------------------------------------------------------------
  // protected
  void SpaClient::pushToSpa (const Event &event) {
    PIMPL_D (SpaClient);

    d->pushToSpa (event);
  }

  //----------------------------------------------------------------------------
  // protected
  bool SpaClient::pullFromSpa (Event &event) {
    PIMPL_D (SpaClient);

    return d->pullFromSpa (event);
  }

  //----------------------------------------------------------------------------
  // protected
  bool SpaClient::pollSpaEvents () {
    PIMPL_D (SpaClient);

    return d->pollSpaEvents();
  }

  //----------------------------------------------------------------------------
  // protected
  uint16_t SpaClient::spaValue (Event::Type type) const {
    PIMPL_D (const SpaClient);

    return d->spaValue (type);
  }

  //----------------------------------------------------------------------------
  void SpaClient::subscribe (std::initializer_list<Event::Type> subscribedEvents) {

    subscribe (std::set<Event::Type> (subscribedEvents));
  }

  //----------------------------------------------------------------------------
  void SpaClient::subscribe (std::set<Event::Type> subscribedEvents) {
    PIMPL_D (SpaClient);

    d->subscribedEvents = subscribedEvents;
  }

  //----------------------------------------------------------------------------
  void SpaClient::subscribe (Event::Type event) {
    PIMPL_D (SpaClient);

    d->subscribedEvents.insert (event);
  }

  //----------------------------------------------------------------------------
  bool SpaClient::isSubscribed (const Event::Type &event) const {
    PIMPL_D (const SpaClient);

    return d->subscribedEvents.empty() || d->subscribedEvents.count (event) > 0;
  }

  //----------------------------------------------------------------------------
  const std::set<Event::Type> &SpaClient::subscribedEvents() const {
    PIMPL_D (const SpaClient);

    return d->subscribedEvents;
  }

  //----------------------------------------------------------------------------
  int SpaClient::available() const {
    PIMPL_D (const SpaClient);

    return d->outQ.size();
  }

  //----------------------------------------------------------------------------
  bool SpaClient::begin () {
    PIMPL_D (SpaClient);

    d->isopen = true;
    return true;
  }

  //---------------------------------------------------------------------------
  bool SpaClient::isOpen () const {
    PIMPL_D (const SpaClient);

    return d->isopen;
  }

  //----------------------------------------------------------------------------
  void SpaClient::end () {
    PIMPL_D (SpaClient);

    d->isopen = false;
  }

  //----------------------------------------------------------------------------
  bool SpaClient::handle () {
    PIMPL_D (SpaClient);

    return d->pollSpaEvents();
  }

  //----------------------------------------------------------------------------
  const String &SpaClient::className() const {
    PIMPL_D (const SpaClient);

    return d->className;
  }
}