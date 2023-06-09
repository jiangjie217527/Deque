#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
//4000000
namespace sjtu {
int cnt;
template <class T> class deque {
public:
  int size_q,block_size_head,block_size_tail;
  int head_block,tail_block;
  size_t tag;
  T ***arr_tail,***arr_head;
  int *size_tail,*size_head,*sum_head,*sum_tail;
  class const_iterator;
  class iterator{
  public:
  /*
  deque
  */
    int size_q,block_size_head,block_size_tail;
    int head_block,tail_block;
    size_t tag;
    T ***arr_tail,***arr_head;
    int *size_tail,*size_head,*sum_head,*sum_tail;
  /*
  deque
  */   
    int cur;
  public:
    iterator(){
      size_q=block_size_head=tag=
      block_size_tail=head_block=tail_block=0;cur=-1;
      size_tail=size_head=sum_head=
      sum_tail=nullptr;
      arr_tail=arr_head=nullptr;
    }
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     */
    iterator operator+(const int &n) const {
      if(cur+n>size_q)throw "undefined";
      if(n<0)return *this-(-n);
      iterator tmp(*this);
      tmp.cur+=n;
      return tmp;
    }
    iterator operator-(const int &n) const {
      if(cur-n<0)throw "undefined";
      if(n<0)return *this+(-n);
      iterator tmp(*this);
      tmp.cur-=n;
      return tmp;
    }

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {
      if(tag!=rhs.tag)throw "invaild_iterator";
      return cur-rhs.cur;
    }
    iterator &operator+=(const int &n) {
        *this=*this+n;
        return *this;
    }
    iterator &operator-=(const int &n) {
        *this=*this-n;
        return *this;
    }

    /**
     * iter++
     */
    iterator operator++(int) {
      iterator tmp(*this);
      *this+=1;
      return tmp;
    }
    /**
     * ++iter
     */
    iterator &operator++() {
      return *this+=1;
    }
    /**
     * iter--
     */
    iterator operator--(int) {
      iterator tmp(*this);
      *this-=1;
      return tmp;
    }
    /**
     * --iter
     */
    iterator &operator--() {
      return *this-=1;
    }

    /**
     * *it
     */
    T &operator*() const {
      if(cur>=size_q||cur<0)throw "invalid";
      if(cur<sum_head[head_block]){
        int l=0,r=(size_head[head_block]==0?head_block-1:head_block);
        while(l<r){
          int m=(l+r)>>1;
          if(sum_head[head_block]-sum_head[m]<=cur)r=m;
          else l=m+1;
        }
        return *arr_head[l][size_head[l]-1-(cur-(sum_head[head_block]-sum_head[l]))];
      }
      else {
        int tmp=cur-sum_head[head_block];
        int l=0,r=(size_tail[tail_block]==0?tail_block-1:tail_block);
        while(l<r){
          int m=(l+r)>>1;
          if(sum_tail[m]>tmp)r=m;
          else l=m+1;
        }
        return *arr_tail[l][tmp-(l==0?0:sum_tail[l-1])];
      }
    }
    /**
     * it->field
     */
    T *operator->() const noexcept {
      return &(operator *());
    }

    bool operator==(const iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    bool operator==(const const_iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    bool operator!=(const iterator &rhs) const {
      return !((*this)==rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !((*this)==rhs);
    }
  };

  class const_iterator {
    public:
  /*
  deque
  */
    int size_q,block_size_head,block_size_tail;
    int head_block,tail_block;
    size_t tag;
    T ***arr_tail,***arr_head;
    int *size_tail,*size_head,*sum_head,*sum_tail; 
  /*
  deque
  */   
    int cur;

    const_iterator(){
      size_q=block_size_head=tag=
      block_size_tail=head_block=tail_block=0;cur=-1;
      size_tail=size_head=sum_head=
      sum_tail=nullptr;
      arr_tail=arr_head=nullptr;
    }
    const_iterator(iterator& rhs){
      block_size_head=rhs.block_size_head;
      block_size_tail=rhs.block_size_tail;
      size_q=rhs.size_q;
      head_block=rhs.head_block;
      tail_block=rhs.tail_block;
      tag=rhs.tag;  
      arr_head=rhs.arr_head;
      arr_tail=rhs.arr_tail;
      size_head=rhs.size_head;
      size_tail=rhs.size_tail;
      sum_tail=rhs.sum_tail;
      sum_head=rhs.sum_tail;
      cur=rhs.cur;
    }
    const_iterator operator+(const int &n) const {
      if(cur+n>size_q)throw "undefined";
      const_iterator tmp(*this);
      tmp.cur+=n;
      return tmp;
    }
    const_iterator operator-(const int &n) const {
      if(cur-n<0)throw "undefined";
      const_iterator tmp(*this);
      tmp.cur-=n;
      return tmp;
    }
    const_iterator &operator+=(const int &n) {
        *this=*this+n;return *this;
    }
    const_iterator &operator-=(const int &n) {
        *this=*this-n;return *this;
    }
    int operator-(const iterator &rhs) const {
      if(tag!=rhs.tag)throw "invaild_iterator";
      return cur-rhs.cur;
    }
    int operator-(const const_iterator &rhs) const {
      if(tag!=rhs.tag)throw "invaild_iterator";
      return cur-rhs.cur;
    }
    const T *operator->() const noexcept {
      return &(operator *());
    }
    const T &operator*() const {
      if(cur>=size_q||cur<0)throw "invalid";
      if(cur<sum_head[head_block]){
        int l=0,r=(size_head[head_block]==0?head_block-1:head_block);
        while(l<r){
          int m=(l+r)>>1;
          if(sum_head[head_block]-sum_head[m]<=cur)r=m;
          else l=m+1;
        }
        return *arr_head[l][size_head[l]-1-(cur-(sum_head[head_block]-sum_head[l]))];
      }
      else {
        int tmp=cur-sum_head[head_block];
        int l=0,r=(size_tail[tail_block]==0?tail_block-1:tail_block);
        while(l<r){
          int m=(l+r)>>1;
          if(sum_tail[m]>tmp)r=m;
          else l=m+1;
        }
        return *arr_tail[l][tmp-(l==0?0:sum_tail[l-1])];
      }
    }
        /**
     * iter++
     */
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      *this+=1;
      return tmp;
    }
    /**
     * ++iter
     */
    const_iterator &operator++() {
      return *this+=1;
    }
    /**
     * iter--
     */
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      *this-=1;
      return tmp;
    }
    /**
     * --iter
     */
    const_iterator &operator--() {
      return *this-=1;
    }
    bool operator==(const iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    bool operator==(const const_iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    bool operator!=(const iterator &rhs) const {
      return !((*this)==rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !((*this)==rhs);
    }
  };
  /**
   * constructors.
   */
  deque() {
    block_size_head=block_size_tail=2;
    size_q=head_block=tail_block=0;
    tag=++cnt;
    arr_tail=new T**[block_size_tail];
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    arr_head=new T**[block_size_head];
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T*[block_size_tail];
      arr_head[i]=new T*[block_size_head];
      size_head[i]=size_tail[i]=
      sum_head[i]=sum_tail[i]=0;
    }
  }
  deque(const deque &rhs) {
    block_size_head=rhs.block_size_head;
    block_size_tail=rhs.block_size_tail;
    size_q=rhs.size_q;
    head_block=rhs.head_block;
    tail_block=rhs.tail_block;
    tag=++cnt;
    arr_tail=new T**[block_size_tail];
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    arr_head=new T**[block_size_head];
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T*[block_size_head];
      size_head[i]=rhs.size_head[i];
      sum_head[i]=rhs.sum_head[i];
      for(int j=0;j<size_head[i];j++)
        arr_head[i][j]=new T(*rhs.arr_head[i][j]);
    }
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T*[block_size_tail];
      size_tail[i]=rhs.size_tail[i];
      sum_tail[i]=rhs.sum_tail[i];
      for(int j=0;j<size_tail[i];j++){
        arr_tail[i][j]=new T(*rhs.arr_tail[i][j]);
      }
    }
  }
  ~deque() {
    for(int i=0;i<block_size_head;i++){
        for(int j=0;j<size_head[i];j++)
          delete arr_head[i][j];
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      for(int j=0;j<size_tail[i];j++)
        delete arr_tail[i][j];
      delete[] arr_tail[i];
    }
    delete[] arr_head;
    delete[] arr_tail;
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
  }

  void clear() {
    for(int i=0;i<block_size_head;i++){
        for(int j=0;j<size_head[i];j++)
          delete arr_head[i][j];
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      for(int j=0;j<size_tail[i];j++)
        delete arr_tail[i][j];
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
    delete[] arr_tail;
    delete[] arr_head;
    block_size_head=block_size_tail=2;
    size_q=head_block=tail_block=0;
    arr_tail=new T**[block_size_tail];
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    arr_head=new T**[block_size_head];
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T*[block_size_head];
      arr_tail[i]=new T*[block_size_tail];
      size_head[i]=size_tail[i]=
      sum_head[i]=sum_tail[i]=0;      
    }
  }
  /**
   * assignment operator.
   */

  void db_tail(){
    int tmpblock_size=block_size_tail*2;
    T ***tmparr;
    tmparr=new T**[tmpblock_size];
    for(int i=0;i<tmpblock_size;i++)
      tmparr[i]=new T*[tmpblock_size];
    int k=0;
    for(int i=0;i<block_size_tail;i++){
      for(int j=0;j<size_tail[i];j++){
        tmparr[k/tmpblock_size][k%tmpblock_size]=new T(*arr_tail[i][j]);
        delete arr_tail[i][j];
        k++;
      }
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] sum_tail;
    delete[] arr_tail;
    size_tail=new int[tmpblock_size];
    sum_tail=new int[tmpblock_size]; //origin!!!
    arr_tail=tmparr;
    block_size_tail=tmpblock_size; 
    tail_block=k/tmpblock_size;
    size_tail[tail_block]=k%tmpblock_size;
    if(tail_block>0){
      size_tail[0]=sum_tail[0]=block_size_tail;
      for(int i=1;i<tail_block;i++){
        size_tail[i]=block_size_tail;
        sum_tail[i]=sum_tail[i-1]+block_size_tail;
      }
    }
    
    sum_tail[tail_block]=(tail_block==0?0:sum_tail[tail_block-1])+size_tail[tail_block];
    for(int i=tail_block+1;i<block_size_tail;i++){
      sum_tail[i]=size_tail[i]=0;
    }
  }

  void db_head(){
    int tmpblock_size=block_size_head*2;
    T ***tmparr;
    tmparr=new T**[tmpblock_size];
    for(int i=0;i<tmpblock_size;i++)
      tmparr[i]=new T*[tmpblock_size];
    int k=0;
    for(int i=0;i<block_size_head;i++){
      for(int j=0;j<size_head[i];j++){
        tmparr[k/tmpblock_size][k%tmpblock_size]=new T(*arr_head[i][j]);
        k++;
        delete arr_head[i][j]; 
      }
      delete[] arr_head[i];
    }
    delete[] size_head;
    delete[] sum_head;
    delete[] arr_head;
    size_head=new int[tmpblock_size];
    sum_head=new int[tmpblock_size];
    arr_head=tmparr;
    block_size_head=tmpblock_size;
    head_block=k/tmpblock_size;
    size_head[head_block]=k%tmpblock_size;
    if(head_block>0){
      size_head[0]=sum_head[0]=block_size_head;
      for(int i=1;i<head_block;i++){
        size_head[i]=block_size_head;
        sum_head[i]=sum_head[i-1]+block_size_head;
      }
    }

    sum_head[head_block]=(head_block==0?0:sum_head[head_block-1])+size_head[head_block];
    for(int i=head_block+1;i<block_size_head;i++){
      size_head[i]=sum_head[i]=0;
    }    
  }

  deque &operator=(const deque &other) {
    if(arr_head==other.arr_head||arr_tail==other.arr_tail)return *this;
    for(int i=0;i<block_size_head;i++){
      for(int j=0;j<size_head[i];j++)
        delete arr_head[i][j];
      delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      for(int j=0;j<size_tail[i];j++)
        delete arr_tail[i][j];
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
    delete[] arr_head;
    delete[] arr_tail;
    block_size_head=other.block_size_head;
    block_size_tail=other.block_size_tail;
    size_q=other.size_q;
    head_block=other.head_block;
    tail_block=other.tail_block;
    tag=++cnt;
    arr_tail=new T**[block_size_tail];
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    arr_head=new T**[block_size_head];
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T*[block_size_head];
      size_head[i]=other.size_head[i];
      sum_head[i]=other.sum_head[i];
      for(int j=0;j<size_head[i];j++){
        arr_head[i][j]=new T(*other.arr_head[i][j]);
      }
    }
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T*[block_size_tail];
      size_tail[i]=other.size_tail[i];
      sum_tail[i]=other.sum_tail[i];
      for(int j=0;j<size_tail[i];j++){
        arr_tail[i][j]=new T(*other.arr_tail[i][j]);
      }
    }
    return *this;
  }

  iterator begin() {
    iterator tmp;
    tmp.arr_head=arr_head;
    tmp.arr_tail=arr_tail;
    tmp.size_head=size_head;
    tmp.size_tail=size_tail;
    tmp.sum_head=sum_head;
    tmp.sum_tail=sum_tail;
    tmp.size_q=size_q;
    tmp.tag=tag;
    tmp.block_size_head=block_size_head;
    tmp.block_size_tail=block_size_tail;
    tmp.head_block=head_block;
    tmp.tail_block=tail_block;
    tmp.cur=0;
    return tmp;
  }
  const_iterator cbegin() const {
    const_iterator tmp;
    tmp.arr_head=arr_head;
    tmp.arr_tail=arr_tail;
    tmp.size_head=size_head;
    tmp.size_tail=size_tail;
    tmp.sum_head=sum_head;
    tmp.sum_tail=sum_tail;
    tmp.size_q=size_q;
    tmp.tag=tag;
    tmp.block_size_head=block_size_head;
    tmp.block_size_tail=block_size_tail;
    tmp.head_block=head_block;
    tmp.tail_block=tail_block;
    tmp.cur=0;
    return tmp;
  }
  iterator end() {
    iterator tmp;
    tmp.arr_head=arr_head;
    tmp.arr_tail=arr_tail;
    tmp.size_head=size_head;
    tmp.size_tail=size_tail;
    tmp.sum_head=sum_head;
    tmp.sum_tail=sum_tail;
    tmp.cur=tmp.size_q=size_q;
    tmp.tag=tag;
    tmp.block_size_head=block_size_head;
    tmp.block_size_tail=block_size_tail;
    tmp.head_block=head_block;
    tmp.tail_block=tail_block;
    return tmp;
  }
  const_iterator cend() const {
    const_iterator tmp;
    tmp.arr_head=arr_head;
    tmp.arr_tail=arr_tail;
    tmp.size_head=size_head;
    tmp.size_tail=size_tail;
    tmp.sum_head=sum_head;
    tmp.sum_tail=sum_tail;
    tmp.cur=tmp.size_q=size_q;
    tmp.tag=tag;
    tmp.block_size_head=block_size_head;
    tmp.block_size_tail=block_size_tail;
    tmp.head_block=head_block;
    tmp.tail_block=tail_block;
    return tmp;
  }
  T &at(const size_t &pos) {
    if(pos>=size_q||pos<0)throw "index_out_of_bound";
    iterator p=begin();
    p+=pos;
    return *p;
  }
  const T &at(const size_t &pos) const {
    if(pos>=size_q||pos<0)throw "index_out_of_bound";
    const_iterator p=cbegin();
    p+=pos;
    return *p;
  }
  T &operator[](const size_t &pos) {
    return at(pos);
  }
  const T &operator[](const size_t &pos) const {
    return at(pos);
  }
  const T &front() const {
    if(size_q==0)throw "container_is_empty";
    return at(0);
  }
  const T &back() const {
    if(size_q==0)throw "container_is_empty";
    return at(size_q-1);
  }
  bool empty() const {
    if(size_q==0)return true;
    else return false;
  }
  size_t size() const {return size_q;}

  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  iterator insert(iterator pos, const T &value) {
    //std::cout<<pos.cur;
    if(pos.tag!=tag||pos.cur<0||pos.cur>size_q)throw "invalid";
    if(pos.cur==size_q){//initial
      push_back(value);
      int t=pos.cur;
      pos=begin()+t;
      return pos;
    }
    size_q++;pos.size_q++;
    if(pos.cur<sum_head[head_block]){ //insert in the head
      if(head_block==block_size_head-1)db_head();  //no more blocks
      int l=0,r=(size_head[head_block]==0&&head_block>0?head_block-1:head_block);
      while(l<r){
        int m=(l+r)>>1;
        if(sum_head[head_block]-sum_head[m]<=pos.cur)r=m;
        else l=m+1;
      }
      int x=size_head[l]-1-(pos.cur-(sum_head[head_block]-sum_head[l]));//find
      if(size_head[l]!=block_size_head){//half block
        if(size_head[l]==0){
          arr_head[l][0]=new T(value);
        }
        else {
          // if(x==block_size_head-1) no such case
            arr_head[l][size_head[l]]=new T(*arr_head[l][size_head[l]-1]);
            for(int i=size_head[l]-1;i>x+1;i--)
              *arr_head[l][i]=*arr_head[l][i-1];
            *arr_head[l][x+1]=value;     
        }
        size_head[l]++;
        for(int i=l;i<=head_block;i++)
          sum_head[i]++;
        if(l==head_block&&size_head[l]==block_size_head){//then full
          head_block++;
          sum_head[head_block]=sum_head[l];
          if(head_block==block_size_head-1)db_head();
        }
      }
      else if(size_head[l+1]==block_size_head){//both full
        head_block++;//exist
        T **tmp=arr_head[head_block];
        for(int i=head_block;i>l+1;i--){
          arr_head[i]=arr_head[i-1];
          size_head[i]=size_head[i-1];
          sum_head[i]=sum_head[i-1]+1;
        }
        arr_head[l+1]=tmp;
        sum_head[l+1]=sum_head[l]+1;
        size_head[l+1]=1;
        if(x!=size_head[l]-1){
          arr_head[l+1][0]=new T(*arr_head[l][size_head[l]-1]);
          for(int i=size_head[l]-1;i>x+1;i--){
            *arr_head[l][i]=*arr_head[l][i-1];
          }
          *arr_head[l][x+1]=value;
        }
        else {
          arr_head[l+1][0]=new T(value);
        }
        if(head_block==block_size_head-1)db_head();
      }
      else{
        if(x!=size_head[l]-1){
          if(size_head[l+1]!=0){
            arr_head[l+1][size_head[l+1]]=new T(*arr_head[l+1][size_head[l+1]-1]);
            for(int i=size_head[l+1]-1;i>0;i--){
              *arr_head[l+1][i]=*arr_head[l+1][i-1];
            }
            *arr_head[l+1][0]=*arr_head[l][size_head[l]-1];
          }
          else {
            arr_head[l+1][0]=new T(*arr_head[l][size_head[l]-1]);
          }
          for(int i=size_head[l]-1;i>x+1;i--){
            *arr_head[l][i]=*arr_head[l][i-1];
          }
          *arr_head[l][x+1]=value;
        }
        else {
          if(size_head[l+1]==0)arr_head[l+1][0]=new T(value);
          else{
            arr_head[l+1][size_head[l+1]]=new T(*arr_head[l+1][size_head[l+1]-1]);
            for(int i=size_head[l+1]-1;i>0;i--){
              *arr_head[l+1][i]=*arr_head[l+1][i-1];
            }
            *arr_head[l+1][0]=value;            
          }
        }
        size_head[l+1]++;
        for(int i=l+1;i<=head_block;i++){
          sum_head[i]++;
        }
        if(l+1==head_block&&size_head[l+1]==block_size_head){
          head_block++;
          sum_head[head_block]=sum_head[head_block-1];
          if(head_block==block_size_head-1)db_head();
        }
      }
    }
    else{
      if(tail_block==block_size_tail-1)db_tail();      
      int tmp=pos.cur-sum_head[head_block];
      int l=0,r=(size_tail[tail_block]==0&&tail_block>0?tail_block-1:tail_block);
      while(l<r){
        int m=(l+r)>>1;
        if(sum_tail[m]>tmp)r=m;
        else l=m+1;
      }
      int x=tmp-(l==0?0:sum_tail[l-1]);
      if(size_tail[l]!=block_size_tail){
        if(size_tail[l]==0){
          arr_tail[l][0]=new T(value);
        }
        else {
          arr_tail[l][size_tail[l]]=new T(*arr_tail[l][size_tail[l]-1]);
          for(int i=size_tail[l]-1;i>x;i--)
            *arr_tail[l][i]=*arr_tail[l][i-1];
          *arr_tail[l][x]=value;
        }  
        size_tail[l]++;
        for(int i=l;i<=tail_block;i++)
          sum_tail[i]++;
        if(l==tail_block&&size_tail[l]==block_size_tail){
          tail_block++;
          sum_tail[tail_block]=sum_tail[l];
          size_tail[tail_block]=0;
          if(tail_block==block_size_tail-1)db_tail(); 
        }
      }
      else if(size_tail[l+1]==block_size_tail){
        tail_block++;
        T **tmp=arr_tail[tail_block];
        for(int i=tail_block;i>l+1;i--){
          arr_tail[i]=arr_tail[i-1];
          size_tail[i]=size_tail[i-1];
          sum_tail[i]=sum_tail[i-1]+1;
        }
        arr_tail[l+1]=tmp;
        sum_tail[l+1]=sum_tail[l]+1;
        size_tail[l+1]=1;
        arr_tail[l+1][0]=new T(*arr_tail[l][size_tail[l]-1]);
        for(int i=size_tail[l]-1;i>x;i--){
          *arr_tail[l][i]=*arr_tail[l][i-1];
        }
        *arr_tail[l][x]=value;
      }
      else{
        if(size_tail[l+1]!=0){
          arr_tail[l+1][size_tail[l+1]]=new T(*arr_tail[l+1][size_tail[l+1]-1]);
          for(int i=size_tail[l+1]-1;i>0;i--){
            *arr_tail[l+1][i]=*arr_tail[l+1][i-1];
          }
          *arr_tail[l+1][0]=*arr_tail[l][size_tail[l]-1];
        }
        else {
          arr_tail[l+1][0]=new T(*arr_tail[l][size_tail[l]-1]);
        }
        for(int i=size_tail[l]-1;i>x;i--){
          *arr_tail[l][i]=*arr_tail[l][i-1];
        }
        *arr_tail[l][x]=value;
        size_tail[l+1]++;
        for(int i=l+1;i<=tail_block;i++){
          sum_tail[i]++;
        }
        if(l+1==tail_block&&size_tail[l+1]==block_size_tail){
          tail_block++;
          sum_tail[tail_block]=sum_tail[tail_block-1];
          if(tail_block==block_size_tail-1)db_tail(); 
        }
      }
    }
    int t=pos.cur;
    pos=begin()+t;
    return pos;
  }
  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {
    if(pos.tag!=tag||pos.cur<0||pos.cur>=size_q)throw "invalid";
    if(size_q==0)throw "container is empty";
    size_q--;
    if(pos.cur<sum_head[head_block]){
      int l=0,r=(size_head[head_block]==0?head_block-1:head_block);
      while(l<r){
        int m=(l+r)>>1;
        if(sum_head[head_block]-sum_head[m]<=pos.cur)r=m;
        else l=m+1;
      }
      int x=size_head[l]-1-(pos.cur-(sum_head[head_block]-sum_head[l]));

      if(size_head[l]!=1){
        for(int i=x;i<size_head[l]-1;i++){
          *arr_head[l][i]=*arr_head[l][i+1];
        }
        size_head[l]--;
        delete arr_head[l][size_head[l]];
        for(int i=l;i<=head_block;i++){
          sum_head[i]--;
        }
      }
      else{
        T** tmp=arr_head[l];
        delete arr_head[l][0];
        for(int i=l;i<head_block;i++){
          arr_head[i]=arr_head[i+1];
          size_head[i]=size_head[i+1];
          sum_head[i]=sum_head[i+1]-1;
        }
        size_head[head_block]=0;
        sum_head[head_block]=0;
        arr_head[head_block]=tmp;
        if(head_block>0)
          head_block--;
      }
    }
    else{     
      int tmp=pos.cur-sum_head[head_block];
      int l=0,r=(size_tail[tail_block]==0?tail_block-1:tail_block);
      while(l<r){
        int m=(l+r)>>1;
        if(sum_tail[m]>tmp)r=m;
        else l=m+1;
      }
      int x=tmp-(l==0?0:sum_tail[l-1]);

      if(size_tail[l]!=1){
        for(int i=x;i<size_tail[l]-1;i++){
          *arr_tail[l][i]=*arr_tail[l][i+1];
        }
        size_tail[l]--;
        delete arr_tail[l][size_tail[l]];
        for(int i=l;i<=tail_block;i++){
          sum_tail[i]--;
        }
      }
      else {
        T** tmp=arr_tail[l];
        delete arr_tail[l][0];
        for(int i=l;i<tail_block;i++){
          arr_tail[i]=arr_tail[i+1];
          size_tail[i]=size_tail[i+1];
          sum_tail[i]=sum_tail[i+1]-1;
        }
        size_tail[tail_block]=0;
        sum_tail[tail_block]=0;
        arr_tail[tail_block]=tmp;
        if(tail_block>0)
          tail_block--;
      }
    }
    int t=pos.cur;
    pos=begin()+t;
    return pos;
  }
  void push_back(const T &value) {
    arr_tail[tail_block][size_tail[tail_block]]=new T(value);
    size_tail[tail_block]++;
    sum_tail[tail_block]++;
    size_q++;
    if(size_tail[tail_block]==block_size_tail){ 
      if(tail_block!=block_size_tail-1){ 
        tail_block++;
        size_tail[tail_block]=0;
        sum_tail[tail_block]=sum_tail[tail_block-1];
      }
      if(tail_block>=block_size_tail-2)
       db_tail();
    }
  }
  void push_front(const T &value) {
    arr_head[head_block][size_head[head_block]]=new T(value);
    size_head[head_block]++;
    sum_head[head_block]++;
    size_q++;
    if(size_head[head_block]==block_size_head){ 
      if(head_block!=block_size_head-1){ 
        head_block++;
        size_head[head_block]=0;
        sum_head[head_block]=sum_head[head_block-1];
      }
      if(head_block>=block_size_head-2)
       db_head();
    }
  }
  void divide(int tp){
    if(tp==1){ //no tail
      for(int i=size_head[0]-1;i>0;i--){
        push_back(*arr_head[0][i]);
        delete arr_head[0][i];
      }
      delete arr_head[0][0];
      T **tmp=arr_head[0];
      int tmpsize=size_head[0];
      for(int i=0;i<head_block;i++){
        arr_head[i]=arr_head[i+1];
        size_head[i]=size_head[i+1];
        sum_head[i]=sum_head[i+1]-tmpsize;
      }
      size_head[head_block]=sum_head[head_block]=0;
      arr_head[head_block]=tmp;
      if(head_block!=0)
        head_block--;
      size_q-=tmpsize;
    }
    else{//no head
      for(int i=size_tail[0]-1;i>0;i--){
        push_front(*arr_tail[0][i]);
        delete arr_tail[0][i];
      }
      delete arr_tail[0][0];
      T **tmp=arr_tail[0];
      int tmpsize=size_tail[0];
      for(int i=0;i<tail_block;i++){
        arr_tail[i]=arr_tail[i+1];
        size_tail[i]=size_tail[i+1];
        sum_tail[i]=sum_tail[i+1]-tmpsize;
      }
      arr_tail[tail_block]=tmp;
      size_tail[tail_block]=sum_tail[tail_block]=0;
      if(tail_block!=0)
        tail_block--;
      size_q-=tmpsize;
    }
  }
  void pop_back() {
    if(size_q==0)throw "container_is_empty";
    if(size_tail[tail_block]!=0){
      size_tail[tail_block]--;
      sum_tail[tail_block]--;
      delete arr_tail[tail_block][size_tail[tail_block]];
      size_q--;
    }
    else if(tail_block!=0){
      sum_tail[tail_block]=0;
      tail_block--;
      size_tail[tail_block]--;
      sum_tail[tail_block]--;  
      delete arr_tail[tail_block][size_tail[tail_block]];

      size_q--;    
    }
    else divide(1);
  }
  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {
    if(size_q==0)throw "container_is_empty";
    if(size_head[head_block]!=0){
      size_head[head_block]--;
      sum_head[head_block]--;
      delete arr_head[head_block][size_head[head_block]];
      size_q--;
    }
    else if(head_block!=0){
      sum_head[head_block]=0;
      head_block--;
      size_head[head_block]--;
      sum_head[head_block]--; 
      delete arr_head[head_block][size_head[head_block]];
      size_q--;
    }
    else divide(-1);
  }
};
} // namespace sjtu

#endif
