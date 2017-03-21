/** Cache-line padded Simple Spin Lock -*- C++ -*-
 * @file
 * @section License
 *
 * Galois, a framework to exploit amorphous data-parallelism in
 * irregular programs.
 *
 * Copyright (C) 2011, The University of Texas at Austin. All rights
 * reserved.  UNIVERSITY EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES
 * CONCERNING THIS SOFTWARE AND DOCUMENTATION, INCLUDING ANY
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR ANY PARTICULAR PURPOSE,
 * NON-INFRINGEMENT AND WARRANTIES OF PERFORMANCE, AND ANY WARRANTY
 * THAT MIGHT OTHERWISE ARISE FROM COURSE OF DEALING OR USAGE OF
 * TRADE.  NO WARRANTY IS EITHER EXPRESS OR IMPLIED WITH RESPECT TO
 * THE USE OF THE SOFTWARE OR DOCUMENTATION. Under no circumstances
 * shall University be liable for incidental, special, indirect,
 * direct or consequential damages or loss of profits, interruption of
 * business, or related expenses which may arise from use of Software
 * or Documentation, including but not limited to those resulting from
 * defects in Software and/or Documentation, or loss or inaccuracy of
 * data of any kind.  
 *
 * @section Description
 *
 * This contains the basic spinlock padded and aligned to use a cache
 * line.  This implements c++11 lockable and try lockable concept
 *
 * @author Andrew Lenharth <andrew@lenharth.org>
 */

#ifndef GALOIS_RUNTIME_LL_PADDED_LOCK_H
#define GALOIS_RUNTIME_LL_PADDED_LOCK_H

#include "SimpleLock.h"
#include "CacheLineStorage.h"

namespace Galois {
namespace Runtime {
namespace LL {

/// PaddedLock is a spinlock.  If the second template parameter is
/// false, the lock is a noop.
template<bool concurrent>
class PaddedLock;

template<>
class PaddedLock<true> {
  mutable CacheLineStorage<SimpleLock > Lock;

public:
  void lock() const { Lock.get().lock(); }
  bool try_lock() const { return Lock.get().try_lock(); }
  void unlock() const { Lock.get().unlock(); }
};

template<>
class PaddedLock<false> {
public:
  void lock() const {}
  bool try_lock() const { return true; }
  void unlock() const {}
};

}
}
} // end namespace Galois

#endif
