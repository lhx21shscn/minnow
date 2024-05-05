#include "byte_stream.hh"

#include <iostream>
#include <cstdint>
#include <string>
#include <memory>
#include <string_view>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) 
: capacity_( capacity ), capacity_now_( capacity ) {}

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

  if (pos == 0) return;

  num_bytes_pushed_ += pos;
  capacity_now_ -= pos;
  shared_ptr<string> ptr = make_shared<string>(data, 0, pos);
  ptr_channel_.push( ptr );
  if (ptr_channel_.size() == 1) {
    s_view_ = string_view(ptr.get()->c_str(), pos);
  }
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
  // return this->data_channel_;
  return s_view_;
}

void Reader::pop( uint64_t len )
{
  len = min(len, bytes_buffered());
  uint64_t temp_len = len;
  // Your code here.
  while ( len > 0 )
  {
    auto s_size = s_view_.size();
    if ( s_size <= len ) {
      ptr_channel_.pop();
      len -= s_size;
      s_view_ = ptr_channel_.empty() ? string_view() : string_view(ptr_channel_.front().get()->c_str(), ptr_channel_.front().get()->size());
    } else {
      s_view_.remove_prefix(len);
      break;
    }
  }
  capacity_now_ += temp_len;
  num_bytes_poped_ += temp_len;
  return;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return capacity_ - capacity_now_;
}