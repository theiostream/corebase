/* CFRuntime.h
   
   Copyright (C) 2010 Free Software Foundation, Inc.
   
   Written by: Stefan Bidigaray
   Date: January, 2010
   
   This file is part of CoreBase.
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; see the file COPYING.LIB.
   If not, see <http://www.gnu.org/licenses/> or write to the 
   Free Software Foundation, 51 Franklin Street, Fifth Floor, 
   Boston, MA 02110-1301, USA.
*/

#ifndef __CFRuntime_h_GNUSTEP_COREBASE_INCLUDE
#define __CFRuntime_h_GNUSTEP_COREBASE_INCLUDE

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFDictionary.h>

CF_EXTERN_C_BEGIN

/** @defgroup CFRuntime CoreBase Runtime
 *  @{
 */

enum
{
  _kCFRuntimeNotATypeID = 0,
  _kCFRuntimeScannedObject = (1UL << 0),
  _kCFRuntimeResourcefulObject = (1UL << 2)
};

typedef struct __CFRuntimeClass CFRuntimeClass;
struct __CFRuntimeClass
{
  CFIndex version;
  const char *className;
  void (*init)(CFTypeRef cf);
  CFTypeRef (*copy)(CFAllocatorRef allocator, CFTypeRef cf);
#if OS_API_VERSION(MAC_OS_X_VERSION_10_2, GS_API_LATEST)
  void (*finalize)(CFTypeRef cf);
#else
  void (*dealloc)(CFTypeRef cf);
#endif
  Boolean (*equal)(CFTypeRef cf1, CFTypeRef cf2);
  CFHashCode (*hash)(CFTypeRef cf);
  CFStringRef (*copyFormattingDesc)(CFTypeRef cf, CFDictionaryRef formatOptions);
  CFStringRef (*copyDebugDesc)(CFTypeRef cf);
#if 0 // FIXME: OS_API_VERSION(MAC_OS_X_VERSION_10_5, GS_API_LATEST)
#define CF_RECLAIM_AVAILABLE 1
  void (*reclaim)(CFTypeRef cf);
#endif
};



/** Registers a new CF class with the runtime.  This function locks the
    class table and so is thread-safe.
    
    @param cls A constant CFRuntimeClass.
    @see _CFRuntimeUnregisterClassWithTypeID()
    @return The next available CFTypeID or _kCFRuntimeNotATypeID
    if none are available.
 */
CFTypeID
_CFRuntimeRegisterClass (const CFRuntimeClass * const cls);

/** Gets the class structure associated with the @a typeID.
    
    @param typeID A CFTypeID to look up.
    @return The CFRuntimeClass for the @typeID
 */
const CFRuntimeClass *
_CFRuntimeGetClassWithTypeID (CFTypeID typeID);

/** Unregisters a class.
    @warning This function is not thread-safe.
    
    @param typeID The CFTypeID to unregister.
    @see _CFRuntimeRegisterClass()
 */
void
_CFRuntimeUnregisterClassWithTypeID (CFTypeID typeID);



typedef struct __CFRuntimeBase CFRuntimeBase;
struct __CFRuntimeBase
{
  void *_isa;
  int16_t _typeID;
  struct
    {
      int16_t ro:       1; // 0 = read-only object
      int16_t reserved: 7; // For internal CFRuntime use
      int16_t info:     8; // Can be used by CF type
    } _flags;
};

#define INIT_CFRUNTIME_BASE(...) { 0, 0, { 1, 0, 0 } }

/** Creates a new CF instance.
    
    @param allocator The CFAllocatorRef to use or NULL for the default
    allocator.
    @param typeID The CFTypeID of the class.
    @param extraBytes The amount of extra bytes over a CFRuntimeBase type
    needed by this instance.
    @param category Currently unused, use NULL.
    @see CFRetain()
    @see CFRelease()
    @return A newly allocator object.
 */
CFTypeRef
_CFRuntimeCreateInstance (CFAllocatorRef allocator, CFTypeID typeID,
                          CFIndex extraBytes, unsigned char *category);

/** Sets the CFTypeID for an instance.
    
    @param cf The object instance to set the type ID.
    @param typeID The new CFTypeID.
 */
void
_CFRuntimeSetInstanceTypeID (CFTypeRef cf, CFTypeID typeID);

/** Initializes a static CF object instance.
    
    @param memory A pointer to a static CF object instance.
    @param typeID The CFTypeID of the instance.
 */
void
_CFRuntimeInitStaticInstance (void *memory, CFTypeID typeID);
#define CF_HAS_INIT_STATIC_INSTANCE 0

/** @}
 */

CF_EXTERN_C_END

#endif /* __CFRuntime_h_GNUSTEP_COREBASE_INCLUDE */
