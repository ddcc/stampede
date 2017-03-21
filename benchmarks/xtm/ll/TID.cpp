/** Thread ID -*- C++ -*-
 * @file
 * @section License
 *
 * Galois, a framework to exploit amorphous data-parallelism in irregular
 * programs.
 *
 * Copyright (C) 2011, The University of Texas at Austin. All rights reserved.
 * UNIVERSITY EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES CONCERNING THIS SOFTWARE
 * AND DOCUMENTATION, INCLUDING ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR ANY
 * PARTICULAR PURPOSE, NON-INFRINGEMENT AND WARRANTIES OF PERFORMANCE, AND ANY
 * WARRANTY THAT MIGHT OTHERWISE ARISE FROM COURSE OF DEALING OR USAGE OF TRADE.
 * NO WARRANTY IS EITHER EXPRESS OR IMPLIED WITH RESPECT TO THE USE OF THE
 * SOFTWARE OR DOCUMENTATION. Under no circumstances shall University be liable
 * for incidental, special, indirect, direct or consequential damages or loss of
 * profits, interruption of business, or related expenses which may arise from use
 * of Software or Documentation, including but not limited to those resulting from
 * defects in Software and/or Documentation, or loss or inaccuracy of data of any
 * kind.
 *
 * @section Description
 *
 * This contains support for thread id.  See TID.h.
 *
 * @author Andrew Lenharth <andrewl@lenharth.org>
 */

#include "ll/TID.h"
#include <cassert>
#include <cstdlib>

__thread unsigned Galois::Runtime::LL::TID = 0;
static unsigned nextID = 0;

namespace {
struct AtomicNextId {
  unsigned next() {
    return __sync_fetch_and_add(&nextID, 1);
  }
};
typedef AtomicNextId NextId;
}

static NextId next;

void Galois::Runtime::LL::initTID() {
  TID = next.next();
  if (TID > 128)
    abort();
}
