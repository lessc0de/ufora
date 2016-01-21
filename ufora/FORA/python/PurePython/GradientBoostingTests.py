#   Copyright 2015 Ufora Inc.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.


from pyfora.algorithms.regressionTrees.GradientBoostedRegressorBuilder \
    import GradientBoostedRegressorBuilder
import pyfora.typeConverters.PurePandas as PurePandas
import pyfora.algorithms.regressionTrees.RegressionTree as RegressionTree


import numpy


def generateData(mbOfData, nColumns):
    nRows = mbOfData * 1024 * 1024 / 8 / (nColumns + 1)
    nRows = int(nRows)
    
    dataVectors = [
        [float(rowIx % (colIx + 2)) for rowIx in xrange(nRows)] \
        for colIx in xrange(nColumns)]

    predictors = PurePandas.PurePythonDataFrame(dataVectors[:-1])
    responses = PurePandas.PurePythonDataFrame(dataVectors[-1:])

    return predictors, responses


class GradientBoostingTests(object):
    def test_gradient_boosting_regression_1(self):
        # verified against old fora implementation

        def f():
            x, y = generateData(0.1, 10)
            
            builder = GradientBoostedRegressorBuilder(1, 1, 1.0)
            fit = builder.fit(x, y)
            return fit.additiveRegressionTree.trees
            

        trees = self.evaluateWithExecutor(f)

        self.assertEqual(len(trees), 2)

        self.assertEqual(len(trees[0].rules), 1)
        self.assertIsInstance(trees[0].rules[0], RegressionTree.RegressionLeafRule)
        self.assertTrue(numpy.isclose(trees[0].rules[0].leafValue, 4.98992443325))
        
        self.assertEqual(len(trees[1].rules), 3)

        self.assertEqual(trees[1].rules[0].jumpIfLess, 1)
        self.assertEqual(trees[1].rules[0].jumpIfHigher, 2)
        self.assertTrue(
            numpy.isclose(trees[1].rules[0].leafValue, -1.99858787976183e-16)
            )
        self.assertEqual(trees[1].rules[0].rule.dimension, 8)

        # (3.013221028930496, 3.00115009354123)
        self.assertTrue(
            numpy.isclose(trees[1].rules[0].rule.splitPoint, 3.00115009354123),
            (trees[1].rules[0].rule.splitPoint, 3.00115009354123)
            )
        self.assertTrue(
            numpy.isclose(
                trees[1].rules[0].rule.impurityImprovement,
                0.00093093285723711
                )
            )
        self.assertEqual(trees[1].rules[0].rule.numSamples, 1191)

        self.assertIsInstance(
            trees[1].rules[1],
            RegressionTree.RegressionLeafRule
            )
        self.assertTrue(
            numpy.isclose(trees[1].rules[1].leafValue, 0.0373292355137323)
            )
         
        self.assertIsInstance(
            trees[1].rules[2],
            RegressionTree.RegressionLeafRule
            )
        self.assertTrue(
            numpy.isclose(trees[1].rules[2].leafValue, -0.0249384388516114)
            )
