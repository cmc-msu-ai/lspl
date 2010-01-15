/*
 * DictionariesTest.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */

#include "Tests.h"

#include "../assertions/PatternsAssertions.h"

#include <iostream>
#include <string>

#include <lspl/base/RefCountObject.h>

using lspl::base::Reference;
using lspl::base::WeakReference;

namespace lspl { namespace tests {

class TestObject : public base::RefCountObject {
public:

	TestObject() {
		++ count;
	}

	~TestObject() {
	-- count;
	}

	static uint count;
};

uint TestObject::count = 0;

static void testStrongRefs() {
	assertEquals( TestObject::count, 0 );

	Reference<TestObject> ref( new TestObject() );

	assertEquals( TestObject::count, 1 );
	assertTrue( ref );

	ref = Reference<TestObject>();

	assertEquals( TestObject::count, 0 );
	assertFalse( ref );
}

static void testWeakRefs() {
	assertEquals( TestObject::count, 0 );

	Reference<TestObject> ref( new TestObject() );
	WeakReference<TestObject> wref = ref;

	assertEquals( TestObject::count, 1 );
	assertTrue( ref );
	assertTrue( wref );
	assertTrue( wref.lock() );

	ref = Reference<TestObject>();

	assertEquals( TestObject::count, 0 );
	assertFalse( ref );
	assertFalse( wref );
	assertFalse( wref.lock() );
}

cute::suite refCountObjectsSuite() {
	cute::suite s;

	s += CUTE(testWeakRefs);
	s += CUTE(testStrongRefs);

	return s;
}

} }
