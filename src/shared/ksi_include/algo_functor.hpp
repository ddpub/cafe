#ifndef DMITRY_DERGACHEV_18_03_2008_ALGO_FUNCTOR_H
#define DMITRY_DERGACHEV_18_03_2008_ALGO_FUNCTOR_H

#include <functional>
#include <utility>
#include <algorithm>

template <typename Func>
class destroyProcessor_t {
public:
	destroyProcessor_t(Func func_): func_(func_) {}
	~destroyProcessor_t() {
		func_();
	}
private:
	Func func_;
};

template <typename T>
struct select1st : public std::unary_function<T, typename T::first_type>
{
  typename T::first_type& operator() (T& pair_) const { return pair_.first; }
  const typename T::first_type& operator() (const T& pair_) const { return pair_.first; }
};

template <typename T>
struct select2nd : public std::unary_function<T, typename T::second_type>
{
  typename T::second_type& operator() (T& pair_) const { return pair_.second; }
  const typename T::second_type& operator() (const T& pair_) const { return pair_.second; }
};

template <typename T>
select2nd<T> select2nd_functor()
{
  return select2nd<T>();
}

template<typename InputIterator, typename Predicate, typename UnaryFunction>
UnaryFunction for_each_if(InputIterator first
                        , InputIterator last
                        , Predicate pred
                        , UnaryFunction f)
{
    for(;first != last; ++first)
    {
        if (pred(*first))
        {
            f(*first);
        }
    }
    return f;
}

template <typename T>
struct identical_t : public std::unary_function<T, T>
{
	T operator() (const T& val) const { return val; }
};

template <typename T>
identical_t<T> identical_functor()
{
	return identical_t<T>();
}

template <typename T>
struct approve_t: public std::unary_function<T, bool>
{
	bool operator() (const T& val) const { return true; }
};

template <typename T>
approve_t<T> approve_functor()
{
	return approve_t<T>();
}

template <typename T>
struct negative_t: public std::unary_function<T, bool>
{
	bool operator() (const T& val) const { return false; }
};

template <typename T>
negative_t<T> negative_functor()
{
	return negative_t<T>();
}

template <typename T, typename U>
class generator_t: public std::unary_function<T, U>
{
public:
  generator_t(const U& u_): u_(u_) {}
  U operator() (const T&) const { return u_; }
private:
  U u_;
};

template <typename T, typename U>
generator_t<T,U> generator_functor(const U& u)
{
  return generator_t<T,U>(u);
}


template <typename T>
struct ptr_to_ref : public std::unary_function<T*, T>
{
  const T& operator() (const T* const_ptr) const { return *const_ptr; }
  T& operator() (T* var_ptr) const { return *var_ptr; }
};

template <typename T>
ptr_to_ref<T> ptr_to_ref_functor()
{
	return ptr_to_ref<T>();
}

template <typename T>
struct ref_to_ptr : public std::unary_function<T, T*>
{
  const T* operator() (const T& const_ref) const { return &const_ref; }
  T* operator() (T& var_ref) const { return &var_ref; }
};

template <typename T>
ref_to_ptr<T> ref_to_ptr_functor()
{
	return ref_to_ptr<T>();
}

template <typename InIter, typename OutIter, typename Pred>
void copy_ptr_if(InIter first_, InIter last_, OutIter out_, Pred cond_)
{
	for ( ; first_ != last_; ++first_)
		if (cond_(*first_)) *out_++ = ref_to_ptr<typename InIter::value_type>()(*first_); //&(*first_);
}

template <typename InIter, typename OutIter, typename Pred>
void copy_if(InIter first_, InIter last_, OutIter out_, Pred cond_)
{
  for ( ; first_ != last_; ++first_)
    if (cond_(*first_)) *out_++ = *first_;
}

template <typename InIter, typename IntType, typename Binary_op, typename Proc, typename Pred>
IntType accumulate_proc_if(InIter first_, InIter last_, IntType val_, Binary_op op_, Proc proc_, Pred cond_)
{
	for ( ; first_ != last_; ++first_)
		if (cond_(*first_)) val_ = op_(val_, proc_(*first_));
	return val_;
}

template <typename InIter, typename IntType, typename Proc, typename Pred>
IntType accumulate_proc_if(InIter first_, InIter last_, IntType val_, Proc proc_, Pred cond_)
{
	return accumulate_proc_if(first_, last_, val_, std::plus<IntType>(), proc_, cond_);
}

template <typename InIter, typename IntType, typename Binary_op, typename Proc>
IntType accumulate_proc(InIter first_, InIter last_, IntType val_, Binary_op op_, Proc proc_)
{
	return accumulate_proc_if(first_, last_, val_, op_, proc_, approve_t<typename InIter::value_type>());
}

template <typename InIter, typename IntType, typename Proc>
IntType accumulate_proc(InIter first_, InIter last_, IntType val_, Proc proc_)
{
	return accumulate_proc_if(first_, last_, val_, std::plus<IntType>(), proc_, approve_t<typename InIter::value_type>());
}

template <typename InIter, typename IntType, typename Binary_op, typename Pred>
IntType accumulate_if(InIter first_, InIter last_, IntType val_, Binary_op op_, Pred cond_)
{
	return accumulate_proc_if(first_, last_, val_, op_, identical_t<typename InIter::value_type>(), cond_);
}

template <typename InIter, typename IntType, typename Pred>
IntType accumulate_if(InIter first_, InIter last_, IntType val_, Pred cond_)
{
	return accumulate_proc_if(first_, last_, val_, std::plus<IntType>(), identical_t<typename InIter::value_type>(), cond_);
}

template <typename InIter, typename OutIter, typename Pred, typename Proc>
OutIter transform_if(InIter first_, InIter last_, OutIter dest_, Pred pred_, Proc proc_)
{
  for (InIter i = first_; i != last_; ++i)
  {
    if (pred_(*i))
    {
      *dest_++ = proc_(*i);
    }
  }
  return dest_;
}

#endif
