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
  //                            Client
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Client::Private::Private (const char *className, Client *q) :
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
  Client::Private::Private (const char *className, const std::set<Event::Type> &sEvents, Client *q) :
    Private (className, q) {

    subscribedEvents = sEvents;
  }
  //----------------------------------------------------------------------------
  void Client::Private::pushToSpa (const Event &event) {

    if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {

      if (subscribedEvents.empty() || subscribedEvents.count (event.type()) > 0) {

        outQ.push (event);
      }
    }
  }

  //----------------------------------------------------------------------------
  bool Client::Private::pullFromSpa (Event &event) {

    if (! inQ.empty()) {

      event = inQ.front();
      inQ.pop();
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool Client::Private::pollSpaEvents () {
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
  uint16_t Client::Private::spaValue (Event::Type type) const {

    if (spaValues.count (type) > 0) {

      return spaValues.at(type);
    }
    return UnsetValue16;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Client::Client (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Client::Client() :
    Client (*new Private ("Client")) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  Client::~Client() {}

  //----------------------------------------------------------------------------
  Client::Client (std::initializer_list<Event::Type> subscribedEvents) :
    Client (*new Private ("Client", std::set<Event::Type> (subscribedEvents)))
  {}

  //----------------------------------------------------------------------------
  Client::Client (const std::set<Event::Type> &subscribedEvents) :
    Client (*new Private ("Client", subscribedEvents))
  {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  Client::Client (const Client &other) :
    Client (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Client &Client::operator= (const Client &other) {

    if (this != &other) {
      PIMPL_D (Client);

      d->outQ = other.d_func()->outQ;
      d->inQ = other.d_func()->inQ;
      d->subscribedEvents = other.d_func()->subscribedEvents;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  Client::Client (Client &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  Client &Client::operator= (Client &&other) = default;

  //----------------------------------------------------------------------------
  bool Client::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool Client::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const Client);

      return d->outQ.empty() && d->inQ.empty() && d->subscribedEvents.empty();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Client::clear() {

    if (isNull()) {

      d_ptr.reset (new Private ("Client"));
    }
    else {

      std::queue<Event>().swap (d_ptr->outQ); // Clear the queue, std::queue::clear() is not available in C++11
      std::queue<Event>().swap (d_ptr->inQ); // Clear the queue
      d_ptr->subscribedEvents.clear();
    }
  }

  //----------------------------------------------------------------------------
  bool Client::operator== (const Client &other) const {
    PIMPL_D (const Client);

    return d->outQ == other.d_func()->outQ &&
           d->inQ == other.d_func()->inQ &&
           d->subscribedEvents == other.d_func()->subscribedEvents &&
           d->className == other.d_func()->className; // TODO: compare the class name ?
  }

  //----------------------------------------------------------------------------
  bool Client::operator!= (const Client &other) const {
    PIMPL_D (const Client);

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  bool Client::write (const Event &event) {

    if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {
      PIMPL_D (Client);

      if (d->subscribedEvents.empty() || d->subscribedEvents.count (event.type()) > 0) {

        d->inQ.push (event);
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------------------------
  Event Client::read () {
    PIMPL_D (Client);
    Event event;

    if (available()) {

      event = d->outQ.front();
      d->outQ.pop();
    }
    return event;
  }

  //----------------------------------------------------------------------------
  Event Client::read (Event::Type type) {
    PIMPL_D (Client);
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
  void Client::pushToSpa (const Event &event) {
    PIMPL_D (Client);

    d->pushToSpa (event);
  }

  //----------------------------------------------------------------------------
  bool Client::pullFromSpa (Event &event) {
    PIMPL_D (Client);

    return d->pullFromSpa (event);
  }

  //----------------------------------------------------------------------------
  bool Client::pollSpaEvents () {
    PIMPL_D (Client);

    return d->pollSpaEvents();
  }

  //----------------------------------------------------------------------------
  uint16_t Client::spaValue (Event::Type type) const {
    PIMPL_D (const Client);

    return d->spaValue (type);
  }

  //----------------------------------------------------------------------------
  void Client::subscribe (std::initializer_list<Event::Type> subscribedEvents) {

    subscribe (std::set<Event::Type> (subscribedEvents));
  }

  //----------------------------------------------------------------------------
  void Client::subscribe (std::set<Event::Type> subscribedEvents) {
    PIMPL_D (Client);

    d->subscribedEvents = subscribedEvents;
  }

  //----------------------------------------------------------------------------
  void Client::subscribe (Event::Type event) {
    PIMPL_D (Client);

    d->subscribedEvents.insert (event);
  }

  //----------------------------------------------------------------------------
  bool Client::isSubscribed (const Event::Type &event) const {
    PIMPL_D (const Client);

    return d->subscribedEvents.empty() || d->subscribedEvents.count (event) > 0;
  }

  //----------------------------------------------------------------------------
  const std::set<Event::Type> &Client::subscribedEvents() const {
    PIMPL_D (const Client);

    return d->subscribedEvents;
  }

  //----------------------------------------------------------------------------
  int Client::available() const {
    PIMPL_D (const Client);

    return d->outQ.size();
  }

  //----------------------------------------------------------------------------
  bool Client::begin () {
    PIMPL_D (Client);

    d->isopen = true;
    return true;
  }

  //---------------------------------------------------------------------------
  bool Client::isOpen () const {
    PIMPL_D (const Client);

    return d->isopen;
  }

  //----------------------------------------------------------------------------
  void Client::end () {
    PIMPL_D (Client);

    d->isopen = false;
  }

  //----------------------------------------------------------------------------
  bool Client::handle () {
    PIMPL_D (Client);

    return d->pollSpaEvents();
  }

  //----------------------------------------------------------------------------
  const String &Client::className() const {
    PIMPL_D (const Client);

    return d->className;
  }
}