#include<iostream>
#include<cmath>
int cnt;
template <class T> class deque {
public:
  int size_q,block_size;
  size_t tag;
  T **arr;
  int *type;int size_head,size_tail,tail_block;
  class const_iterator;
  class iterator {
  public:
    int x,y;
    int size,block_size;
    int *type;int size_head,size_tail;//6+2+1
    size_t tag; T **arr;
  public:
    iterator(){
      x=y=size=block_size=tag=size_head=size_tail=tail_block=0;
      arr=type=nullptr;
    }
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     */
    iterator operator+(const int &n) const {
      if(x*block_size+(type[x]==1?y:block_size-1-y)+n>size-1)throw "undefined";
      iterator tmp(*this);
      int dis;
      if(type[x]==1){
        dis=block_size-1-y;
      }
      else{
        dis=y;
      }
      if(n<=dis){
        y+=(type[x]*n);
      }
      else {
        n-=dis;
        tmp.x+=(1+n/block_size);
        if(type[tmp.x]==1){
          tmp.y=n%block_size;
        }
        else{
          tmp.y=block_size-1-n%block_size;
        }
      }
      return tmp;
    }
    iterator operator-(const int &n) const {
      if(x*block_size+(type[x]==1?y:block_size-1-y)-n<0)throw "undefined";
      iterator tmp(*this);
      int dis;
      if(type[x]==-1){
        dis=block_size-1-y;
      }
      else{
        dis=y;
      }
      if(n<=dis){
        y-=(type[x]*n);
      }
      else {
        n-=dis;
        tmp.x-=(2+n/block_size);
        if(type[tmp.x]==-1){
          tmp.y=n%block_size;
        }
        else{
          tmp.y=block_size-1-n%block_size;
        }            
      }
      return tmp;
    }

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {
      if(tag!=rhs.tag)throw "invaild_iterator";
      if(x==rhs.x){
        return (y-rhs.y)*type[x];
      }
      else if(x>rhs.x){
        int dis1,dis2;
        dis1=(type[x]==1?block_size-1-y:y);
        dis2=(type[rhs.x]==-1?block_size-1-y:y);
        return dis1+dis2+(x-rhs.x-1)*block_size;
      }
      else{
        return rhs-*this;
      }
    }
    iterator &operator+=(const int &n) {
        if(x*block_size+(type[x]==1?y:block_size-1-y)+n>size-1)throw "undefined";
        *this=*this+n;
        return *this;
    }
    iterator &operator-=(const int &n) {
        if(x*block_size+(type[x]==1?y:block_size-1-y)-n<0)throw "undefined";
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
      return arr[x][y];
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
      if(tag!=rhs.tag)return false;
      if(x!=rhs.x||y!=rhs.y)return false;
      return true;
    }
    bool operator==(const const_iterator &rhs) const {
      if(tag!=rhs.tag)return false;
      if(x!=rhs.x||y!=rhs.y)return false;
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
      std::cout<<x<<" "<<y;
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
    block_size=2;size_q=0;
    tag=++cnt;
    arr=new T*[block_size];
    for(int i=0;i<block_size;i++){
        arr[i]=new T[block_size];
    }
  }
  deque(const deque &rhs) {
    size=rhs.size;block_size=rhs.block_size;
    tag=rhs.tag;arr=rhs.arr;
  }

  /**
   * deconstructor.
   */
  ~deque() {
    for(int i=0;i<block_size;i++){
        delete[] arr[i];
    }
    delete [] arr;
  }

  void clear() {
    for(int i=0;i<block_size;i++){
        delete[] arr[i];
    }
    block_size=1;size_q=0;
    for(int i=0;i<block_size;i++){
        arr[i]=new T[block_size];
    } 
  }
  /**
   * assignment operator.
   */

  void db(){
    int tmpblock_size=block_size*2;
    T ** tmparr;
    tmparr=new T*[tmpblock_size];
    for(int i=0;i<tmpblock_size;i++){
        tmparr[i]=new T[tmpblock_size];
    }
    for(int i=0;i<size_q;i++){
      tmparr[i/tmpblock_size][i%tmpblock_size]=arr[i/block_size][i%block_size];
    }
    for(int i=0;i<block_size;i++){
        delete[] arr[i];
    }
    delete arr;
    arr=tmparr;
    block_size=tmpblock_size;
  }

  deque &operator=(const deque &other) {
    for(int i=0;i<block_size;i++){
        delete[] arr[i];
    }
    block_size=other.block_size;size_q=other.size_q;
    tag=++cnt;
    for(int i=0;i<block_size;i++){
        arr[i]=new T[block_size];
    }
    int X=size_q/block_size,Y=size_q%block_size;
    for(int i=0;i<=X;i++){
      for(int j=0;j<block_size&&i*block_size+j<size_q;j++){
        arr[i][j]=other.arr[i][j];
      }
    }
    return *this;
  }

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos) {
    if(pos>=size_q||pos<0)throw "index_out_of_bound";
    int x=pos/block_size,y=pos%block_size;
    return arr[x][y];
  }
  const T &at(const size_t &pos) const {
    if(pos>=size_q||pos<0)throw "index_out_of_bound";
    int x=pos/block_size,y=pos%block_size;
    return arr[x][y];
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
   * return an iterator to the beginning.
   */
  iterator begin() {
    iterator tmp;
    tmp.x=tmp.y=0;tmp.tag=tag;
    tmp.size=size_q;tmp.block_size=block_size;
    tmp.arr=arr;
    return tmp;
  }
  const_iterator cbegin() const {
    iterator tmp;
    tmp.x=tmp.y=0;tmp.tag=tag;
    tmp.size=size_q;tmp.block_size=block_size;
    tmp.arr=arr;
    return tmp;
  }

  /**
   * return an iterator to the end.
   */
  iterator end() {
    iterator tmp;
    tmp.x=size_q/block_size;
    tmp.y=size_q%block_size;
    tmp.tag=tag;
    tmp.size=size_q;tmp.block_size=block_size;
    tmp.arr=arr;
    return tmp;
  }
  const_iterator cend() const {
    iterator tmp;
    tmp.x=size_q/block_size;
    tmp.y=size_q%block_size;
    tmp.tag=tag;
    tmp.size=size_q;tmp.block_size=block_size;
    tmp.arr=arr;
    return tmp;
  }

  /**
   * check whether the container is empty.
   */
  bool empty() const {
    if(size_q==0)return true;
    else return false;
  }

  /**
   * return the number of elements.
   */
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
    
    if(sqrt(size_q+1)==block_size)
        db();
    
    int x=size_q/block_size,y=size_q%block_size;
    arr[x][y]=value;
    size_q++;
  }

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {
    if(size_q==0)throw "container_is_empty";
    size_q--;
    }

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {}

  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {}
};

int main(){
    deque<long long>q;
    q.push_back(2);
    q.push_back(3);
    q.push_back(3);
    q.push_back(4);
    q.push_back(5);
    deque<long long>qq;
    qq=q;
    std::cout<<qq[4];
    return 0;
}