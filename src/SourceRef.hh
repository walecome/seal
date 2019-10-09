#pragma once
#include "TokenBuffer.hh"

/*
    Reference to the source file tokens. 'end' member is like std::end, it is
    supposed to point to the iterator next outside the reference scope.
*/
struct SourceRef {
    TokenBuffer::Iterator begin;
    TokenBuffer::Iterator end;
};