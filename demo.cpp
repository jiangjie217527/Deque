#include<iostream>
#include<cmath>
int cnt;
template <class T> class deque {
public:
  int size_q,block_size_head,block_size_tail;
  int head_block,tail_block;
  size_t tag;
  T **arr_tail,**arr_head;
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
    T **arr_tail,**arr_head;
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
      if(cur+n>size_q-1)throw "undefined";
      iterator tmp(*this);
      tmp.cur+=n;
      return tmp;
    }
    iterator operator-(const int &n) const {
      if(cur-n<0)throw "undefined";
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
        if(cur+n>size_q-1)throw "undefined";
        *this=*this+n;
        return *this;
    }
    iterator &operator-=(const int &n) {
        if(cur-n<0)throw "undefined";
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
      if(cur<sum_head[head_block]){
        int l=0,r=head_block;
        while(l<r){
          int m=(l+r)>>1;
          if(sum_head[head_block]-sum_head[m]<=cur)r=m;
          else l=m+1;
        }
        return arr_head[l][block_size_head-1-(cur-(sum_head[head_block]-sum_head[l]))];
      }
      else {
        int tmp=cur-sum_head[head_block];
        int l=0,r=tail_block;
        while(l<r){
          int m=(l+r)>>1;
          if(sum_tail[m]>=cur)r=m;
          else l=m+1;
        }
        return arr_tail[l][tmp-(l==0?0:sum_tail[l-1])];
      }
    }
    /**
     * it->field
     */
    T *operator->() const noexcept {
    }

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    bool operator==(const const_iterator &rhs) const {
      if(tag!=rhs.tag||cur!=rhs.cur)return false;
      return true;
    }
    /**
     * some other operator for iterators.
     */
    bool operator!=(const iterator &rhs) const {
      return !((*this)==rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !((*this)==rhs);
    }
    void print(){
      std::cout<<cur;
    }
  };

  class const_iterator {
    public:
    size_t x,y;
    size_t size,block_size;
    size_t tag; T **&arr;
    /**
     * it should has similar member method as iterator.
     * you can copy them, but with care!
     * and it should be able to be constructed from an iterator.
     */
    const_iterator(iterator& rhs){
      x=rhs.x;y=rhs.y;
      size=rhs.size;block_size=rhs.block_size;
      tag=rhs.tag;arr=rhs.arr;
    }
    iterator operator+(const int &n) const {
        if(x*block_size+y+n>size)throw "undefined";
        iterator tmp(*this);
        tmp.y=(y+n)%block_size;
        tmp.x=(x+n/block_size);
        return tmp;
    }
    iterator operator-(const int &n) const {
        if(x*block_size+y-n<0)throw "undefined";
        iterator tmp(*this);
        if(y<n){
          tmp.y=(y-n)%block_size+block_size;
          tmp.x=x-n/block_size-1;
        }
        else {
          tmp.y=y-n;
          tmp.x=x;
        }
        return tmp;
    }
    int operator-(const iterator &rhs) const {
      if(tag!=rhs.tag)throw "invaild_iterator";
      return (x-rhs.x)*block_size+(y-rhs.y);
    }
    T &operator*() const {
      return arr[x][y];
    }
    bool operator==(const iterator &rhs) const {
      if(tag!=rhs.tag)return false;
      if(x!=rhs.x||y!=rhs.y)return false;
      return true;
    }
    bool operator==(const const_iterator &rhs) const {
      if(tag!=rhs.tag)return false;
      if(x!=rhs.x||y!=rhs.y)return false;
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
    arr_tail=new T*[block_size_tail];
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    arr_head=new T*[block_size_head];
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T[block_size_tail];
      arr_head[i]=new T[block_size_head];
    }
    for(int i=0;i<block_size_head;i++){
      size_head[i]=size_tail[i]=
      sum_head[i]=sum_tail[i]=0;
    }
  }
  deque(const deque &rhs) {
    for(int i=0;i<block_size_head;i++){
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
    block_size_head=rhs.block_size_head;
    block_size_tail=rhs.block_size_tail;
    size_q=rhs.size_q;
    head_block=rhs.head_block;
    tail_block=rhs.tail_block;
    tag=++cnt;
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];   
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T[block_size_head];
      size_head[i]=rhs.size_head[i];
      sum_head[i]=rhs.sum_head[i];
      for(int j=0;j<size_head[i];j++)
        arr_head[i][j]=rhs.arr_head[i][j];
    }
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T[block_size_tail];
      size_tail[i]=rhs.size_tail[i];
      sum_tail[i]=rhs.sum_tail[i];
      for(int j=0;j<size_tail[i];j++)
        arr_tail[i][j]=rhs.arr_tail[i][j];            
    }
  }

  /**
   * deconstructor.
   */
  ~deque() {
    for(int i=0;i<block_size_head;i++){
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
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
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
    block_size_head=block_size_tail=2;
    size_q=head_block=tail_block=0;
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T[block_size_head];
      arr_tail[i]=new T[block_size_tail];
      size_head[i]=size_tail[i]=
      sum_head[i]=sum_tail[i]=0;      
    }
  }
  /**
   * assignment operator.
   */

  int db_tail(){
    int tmpblock_size=block_size_tail*2;
    T **tmparr;
    tmparr=new T*[tmpblock_size];
    for(int i=0;i<tmpblock_size;i++){
        tmparr[i]=new T[tmpblock_size];
    }
    int k=0;
    for(int i=0;i<=tail_block;i++){
      for(int j=0;j<size_tail[i];j++){
        tmparr[k/tmpblock_size][k%tmpblock_size]=arr_tail[i][j];
        k++;
      }
    }
    delete[] size_tail;
    delete[] sum_tail;
    size_tail=new int[tmpblock_size];
    sum_tail=new int[tmpblock_size];
    for(int i=0;i<block_size_tail;i++){
        delete[] arr_tail[i];
    }
    delete arr_tail;
    arr_tail=tmparr;
    block_size_tail=tmpblock_size; 

    size_tail[k/tmpblock_size]=k%tmpblock_size;
    return k/tmpblock_size;
  }

  int db_head(){
    int tmpblock_size=block_size_head*2;
    T **tmparr;
    tmparr=new T*[tmpblock_size];
    for(int i=0;i<tmpblock_size;i++){
        tmparr[i]=new T[tmpblock_size];
    }
    int k=0;
    for(int i=0;i<=head_block;i++){
      for(int j=0;j<size_head[i];j++){
        tmparr[k/tmpblock_size][k%tmpblock_size]=arr_head[i][j];
        k++;
      }
    }
    delete[] size_head;
    delete[] sum_head;
    size_head=new int[tmpblock_size];
    sum_head=new int[tmpblock_size];
    for(int i=0;i<block_size_head;i++){
        delete[] arr_head[i];
    }
    delete arr_head;
    arr_head=tmparr;
    block_size_head=tmpblock_size; 

    size_head[k/tmpblock_size]=k%tmpblock_size;
    return k/tmpblock_size;
  }

  deque &operator=(const deque &other) {
    for(int i=0;i<block_size_head;i++){
        delete[] arr_head[i];
    }
    for(int i=0;i<block_size_tail;i++){
      delete[] arr_tail[i];
    }
    delete[] size_tail;
    delete[] size_head;
    delete[] sum_tail;
    delete[] sum_head;
    block_size_head=other.block_size_head;
    block_size_tail=other.block_size_tail;
    size_q=other.size_q;
    head_block=other.head_block;
    tail_block=other.tail_block;
    tag=++cnt;
    size_tail=new int[block_size_tail];
    sum_tail=new int[block_size_tail];
    sum_head=new int[block_size_head];
    size_head=new int[block_size_head];   
    for(int i=0;i<block_size_head;i++){
      arr_head[i]=new T[block_size_head];
      size_head[i]=other.size_head[i];
      sum_head[i]=other.sum_head[i];
      for(int j=0;j<size_head[i];j++)
        arr_head[i][j]=other.arr_head[i][j];
    }
    for(int i=0;i<block_size_tail;i++){
      arr_tail[i]=new T[block_size_tail];
      size_tail[i]=other.size_tail[i];
      sum_tail[i]=other.sum_tail[i];
      for(int j=0;j<size_tail[i];j++)
        arr_tail[i][j]=other.arr_tail[i][j];            
    }
    return *this;
  }

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */

  
  /**
   * return an iterator to the beginning.
   */
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

  /**
   * return an iterator to the end.
   */
  iterator end() {
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
    tmp.cur=size_q-1;
    return tmp;
  }
  const_iterator cend() const {
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
    tmp.cur=size_q-1;
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
    iterator p=begin();
    p+=pos;
    return *p;
  }
  T &operator[](const size_t &pos) {
    return at(pos);
  }
  const T &operator[](const size_t &pos) const {
    return at(pos);
  }

  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
    if(size_q==0)throw "container_is_empty";
    return at(0);
  }
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const {
    if(size_q==0)throw "container_is_empty";
    return at(size_q-1);
  }


  /**
   * check whether the container is empty.
   */
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
  iterator insert(iterator pos, const T &value) {}

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {}

  /**
   * add an element to the end.
   */
  void push_back(const T &value) {
    int x=tail_block;
    int y=size_tail[tail_block];
    arr_tail[x][y]=value;
    size_tail[tail_block]++;
    sum_tail[tail_block]++;
    size_q++;
    if(size_tail[tail_block]==block_size_tail){ 
      if(tail_block!=block_size_tail-1){ 
        tail_block++;
        sum_tail[tail_block]=sum_tail[tail_block-1];
      }
      else {
        tail_block=db_tail();
        if(tail_block>0){
          size_tail[0]=sum_tail[0]=block_size_tail;
          for(int i=1;i<tail_block;i++){
            size_tail[i]=block_size_tail;
            sum_tail[i]=sum_tail[i-1]+block_size_tail;
          }
        }
        sum_tail[tail_block]=(tail_block==0?0:sum_tail[tail_block-1])+size_tail[tail_block];
      }
    }
  }

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {
    if(size_q==0)throw "container_is_empty";
    if(size_tail[tail_block]!=0){
      size_q--;
      size_tail[tail_block]--;
      sum_tail[tail_block]--;
    }
  }

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {
    int x=head_block;
    int y=size_head[head_block];
    arr_head[x][y]=value;
    size_head[head_block]++;
    sum_head[head_block]++;
    size_q++;
    if(size_head[head_block]==block_size_head){ 
      if(head_block!=block_size_head-1){ 
        head_block++;
        sum_head[head_block]=sum_head[head_block-1];
      }
      else {
        head_block=db_head();
        if(head_block>0){
          size_head[0]=sum_head[0]=block_size_head;
          for(int i=1;i<head_block;i++){
            size_head[i]=block_size_head;
            sum_head[i]=sum_head[i-1]+block_size_head;
          }
        }
        sum_head[head_block]=(head_block==0?0:sum_head[head_block-1])+size_head[head_block];
      }
    }
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
      size_q--;
    }
  }
  void print(){
    std::cout<<"size_q:"<<size_q<<std::endl;
    std::cout<<"size_head:"<<size_head[head_block]<<std::endl;
    std::cout<<"size_tail:"<<size_tail[tail_block]<<std::endl;
  }
};

int main(){
    deque<long long>q,qq;
    q.push_front(2);
    q.push_front(3);
    q.push_front(4);
    q.push_front(5);
    q.push_front(4);
    std::cout<<q[1];
    return 0;
}