#include "byte_stream.hh"

#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) 
: capacity_( capacity ), capacity_now_( capacity ) {
  data_channel_.reserve(capacity);
}

bool Writer::is_closed() const
{
  // Your code here.
  return close_;
}

void Writer::push( string data )
{
  // Your code here.
  if (is_closed() || has_error())
    return;
  
  uint64_t s = data.size();
  uint64_t pos = min( s, available_capacity() );
  num_bytes_pushed_ += pos;
  capacity_now_ -= pos;
  // data_channel_.append( move( data.substr( 0, pos ) ) );
  data_channel_.append( data.substr( 0, pos ) );
  return;
}

void Writer::close()
{
  // Your code here.
  close_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_now_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return num_bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return close_ && (capacity_ == capacity_now_);
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return num_bytes_poped_;
}

string_view Reader::peek() const
{
  // Your code here.
  return this->data_channel_;
  // return { string_view( &data_channel_.front(), 1 ) };
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  len = min(len, bytes_buffered());

  capacity_now_ += len;
  num_bytes_poped_ += len;

  data_channel_.erase(0, len);
  return;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return capacity_ - capacity_now_;
}
