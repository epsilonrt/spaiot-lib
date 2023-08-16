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

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            Client
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Client::Private::Private ()
  {}

  //----------------------------------------------------------------------------
  Client::Private::Private (std::set<Event::Type> subscribedEvents) :
    subscribedEvents (subscribedEvents)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Client::Client (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Client::Client() :
    Client (*new Private) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  Client::~Client() {}

  //----------------------------------------------------------------------------
  Client::Client (std::initializer_list<Event::Type> subscribedEvents) :
    Client (*new Private (std::set<Event::Type> (subscribedEvents)))
  {}

  //----------------------------------------------------------------------------
  Client::Client (std::set<Event::Type> subscribedEvents) :
    Client (*new Private (subscribedEvents))
  {}


  // ---------------------------------------------------------------------------
  // Copy constructor
  Client::Client (const Client &other) :
    Client (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Client &Client::operator= (const Client &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
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

      d_ptr.reset (new Private);
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
           d->subscribedEvents == other.d_func()->subscribedEvents;
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

    if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {
      PIMPL_D (Client);

      if (d->subscribedEvents.empty() || d->subscribedEvents.count (event.type()) > 0) {

        d->outQ.push (event);
      }
    }
  }

  //----------------------------------------------------------------------------
  bool Client::pullFromSpa (Event &event) {
    PIMPL_D (Client);

    if (! d->inQ.empty()) {

      event = d->inQ.front();
      d->inQ.pop();
      return true;
    }
    return false;
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
}