/* This file is part of Into.
 * Copyright (C) Intopii 2013.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#ifndef _PIIQUANTIZER_H
#define _PIIQUANTIZER_H

#include <algorithm>
#include <PiiMatrix.h>

/**
 * A class that quantizes (floating point) values to integers. Each
 * quantization level corresponds to a continuous range of values.
 *
 */
template <class T> class PiiQuantizer
{
public:
  /**
   * Create a new quantizer.
   */
  PiiQuantizer() {}
  /**
   * Create a new quantizer with quantization limits. See [setLimits()]
   * for details.
   */
  PiiQuantizer(const PiiMatrix<T>& limits) : _matLimits(limits) {}

  /**
   * Quantize a (floating-point) value to an integer (quantization
   * level index).
   */
  int quantize(T value) const;

  /**
   * Get the number of quantization levels.
   */
  unsigned int levels() const { return _matLimits.columns()+1; }
  /**
   * Get the maximum quantization index. This is a convenience
   * function that returns levels()-1.
   */
  unsigned int maxValue() const { return _matLimits.columns(); }

  /**
   * Set quantization limits. Quantization limits are represented as a
   * row matrix of monotonically increasing numbers.
   *
   * ~~~(c++)
   * PiiQuantizer<float> q;
   * PiiMatrix<float> limits(1,4, 0.0, 0.1, 0.5, 0.7);
   * q.setLimits(limits);
   * q.quantize(0.3); // returns 2
   * q.quantize(-1.0); // returns 0
   * ~~~
   */
  void setLimits(const PiiMatrix<T>& limits) { _matLimits = limits; }
  /**
   * Get the current limits.
   */
  PiiMatrix<T> limits() const { return _matLimits; }

  /**
   * Create limits based on a set of "training" data. Quantization
   * boundaries are derived from the training data so that each
   * quantization range has an equal number of entries.
   *
   * ~~~(c++)
   * PiiMatrix<int> data(1,9, 5, 2, 1, 1, 3, 4, 1, 5, 5);
   * PiiMatrix<int> levels(PiiQuantizer<int>::divideEqually(data, levels));
   * // returns (2, 5)
   * PiiQuantizer<int> q(levels); // converts ints to ints
   * ~~~
   *
   * Note that `data` may be modified. Best performance is attained
   * if `data` is a contiguous matrix.
   */
  static PiiMatrix<T> divideEqually(PiiMatrix<T>& data, int levels);

private:
  PiiMatrix<T> _matLimits;

  static PiiMatrix<T> divideEqually(typename PiiMatrix<T>::iterator begin,
                                    typename PiiMatrix<T>::iterator end,
                                    int levels);
};

template <class T> PiiMatrix<T> PiiQuantizer<T>::divideEqually(PiiMatrix<T>& data, int levels)
{
  return divideEqually(data.begin(), data.end(), levels);
}

template <class T>
PiiMatrix<T> PiiQuantizer<T>::divideEqually(typename PiiMatrix<T>::iterator begin,
                                            typename PiiMatrix<T>::iterator end,
                                            int levels)
{
  std::sort(begin, end);
  double blockSize = (double)(end-begin)/levels;
  PiiMatrix<T> result(1,levels-1);
  for (int i=1; i<levels; i++)
    result(0,i-1) = begin[int(blockSize*i+0.5)];
  return result;
}

template <class T> int PiiQuantizer<T>::quantize(T value) const
{
  int start = 0, end = _matLimits.columns();
  while (start<end)
    {
      int half = (start+end)>>1;
      if (_matLimits(0,half) > value)
        end = half;
      else
        start = half+1;
    }
  return start;
}

#endif //_PIIQUANTIZER_H
