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

#ifndef _PIIDECISIONSTUMP_H
#define _PIIDECISIONSTUMP_H

#include "PiiLearningAlgorithm.h"
#include "PiiClassifier.h"

#include <PiiSerializationTraits.h>

/**
 * A primitive learner that works by thresholding a single feature. A
 * decision stump is trained with weighted samples. The learning
 * algorithm classifies the sample set to two classes by thresholding
 * just one of the features values so that the weighted error is
 * minimized.
 *
 * Traditionally, decision stumps are used binary classification
 * problems. This class implements a more general multi-class decision
 * stump that selects not only the optimal threshold but also two
 * classes that are optimally separated by the threshold.
 *
 */
template <class SampleSet> class PiiDecisionStump :
  public PiiClassifier<SampleSet>,
  public PiiLearningAlgorithm<SampleSet>
{
public:
  typedef typename PiiSampleSet::Traits<SampleSet>::ConstFeatureIterator ConstFeatureIterator;
  typedef typename PiiSampleSet::Traits<SampleSet>::FeatureType FeatureType;

  PiiDecisionStump();

  /**
   * Finds the feature that best separates the two classes present in
   * *samples* and an optimal threshold for it.
   */
  void learn(const SampleSet& samples,
             const QVector<double>& labels,
             const QVector<double>& weights);

  /**
   * Returns [leftLabel()] if the [selectedFeature()] "selected
   * feature" is less than or equal to [threshold()] and [rightLabel()]
   * otherwise.
   */
  double classify(ConstFeatureIterator sample) throw();

  virtual bool converged() const throw ();

  /**
   * Returns `WeightedLearner`.
   */
  virtual PiiClassification::LearnerCapabilities capabilities() const;

  /**
   * Sets the selected feature to *feature*. If the selected feature
   * is set manually, one must ensure it won't exceed the length of
   * the incoming feature vectors.
   */
  void setSelectedFeature(int feature);
  /**
   * Returns the index of the feature that was chosen as the optimal
   * one by [learn()].
   */
  int selectedFeature() const;

  void setThreshold(FeatureType threshold);
  /**
   * Returns the optimal threshold value for the selected feature.
   */
  FeatureType threshold() const;

  void setLeftLabel(double leftLabel);
  double leftLabel() const;

  void setRightLabel(double rightLabel);
  double rightLabel() const;

private:
  /// @internal
  struct Feature
  {
    Feature() : value(0), iLabel(0), dWeight(0) {}
    Feature(FeatureType v, int l, double w) : value(v), iLabel(l), dWeight(w) {}

    FeatureType value;
    int iLabel;
    double dWeight;

    bool operator< (const Feature& other) const { return value < other.value; }
    bool operator> (const Feature& other) const { return value > other.value; }
  };

  /// @internal
  class Data : public PiiLearningAlgorithm<SampleSet>::Data
  {
  public:
    Data();

    int iSelectedFeature;
    double dLeftLabel, dRightLabel;
    FeatureType threshold;
  };
  PII_D_FUNC;

  double optimizeSplit(const QVector<double>& leftWeights,
                       const QVector<double>& weightTotals,
                       double totalWeightSum,
                       int* leftLabel, int* rightLabel);
  friend struct PiiSerialization::Accessor;
  PII_DECLARE_VIRTUAL_METAOBJECT_FUNCTION;
  template <class Archive> void serialize(Archive& archive, const unsigned int)
  {
    PII_D;
    archive & PII_NVP("feature", d->iSelectedFeature);
    archive & PII_NVP("left", d->dLeftLabel);
    archive & PII_NVP("right", d->dRightLabel);
    archive & PII_NVP("threshold", d->threshold);
  }
};

PII_CLASSIFICATION_NAME_ALIAS_AS_T(PiiDecisionStump, PiiMatrix<T>);
PII_DEFINE_VIRTUAL_METAOBJECT_FUNCTION_TEMPLATE(PiiDecisionStump);

#include "PiiDecisionStump-templates.h"

#endif //_PIIDECISIONSTUMP_H
