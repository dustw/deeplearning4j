/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

// 
// created by Yurii Shyrma on 05.12.2017
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_gru_cell)

#include <ops/declarable/CustomOperations.h>
#include<ops/declarable/helpers/gru.h>

namespace nd4j {
namespace ops  {


//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(gru_cell, 5, 1, false, 0, 0) {

    NDArray<T>* x  = INPUT_VARIABLE(0);                     // input [bS x inSize]
    NDArray<T>* h0 = INPUT_VARIABLE(1);                     // previous cell output [bS x numUnits],  that is at previous time step t-1

    NDArray<T>* Wx   = INPUT_VARIABLE(2);                   // input-to-hidden weights, [inSize   x 3*numUnits] 
    NDArray<T>* Wh   = INPUT_VARIABLE(3);                   // hidden-to-hidden weights, [numUnits x 3*numUnits]     
    NDArray<T>* b    = INPUT_VARIABLE(4);                   // biases, [3*numUnits] 
    
    NDArray<T>* h    =  OUTPUT_VARIABLE(0);                  // current cell output [bS x numUnits], that is at current time step t

    const int rank     = x->rankOf();              // = 2
    const int bS       = x->sizeAt(0);
    const int inSize   = x->sizeAt(1);
    const int numUnits = h0->sizeAt(1);

    const std::string h0Shape        = ShapeUtils<T>::shapeAsString(h0);
    const std::string h0CorrectShape = ShapeUtils<T>::shapeAsString({bS, numUnits});
    const std::string wxShape        = ShapeUtils<T>::shapeAsString(Wx);
    const std::string wxCorrectShape = ShapeUtils<T>::shapeAsString({inSize, 3*numUnits});
    const std::string whShape        = ShapeUtils<T>::shapeAsString(Wh);
    const std::string whCorrectShape = ShapeUtils<T>::shapeAsString({numUnits, 3*numUnits});
    const std::string bShape         = ShapeUtils<T>::shapeAsString(b);
    const std::string bCorrectShape  = ShapeUtils<T>::shapeAsString({3*numUnits});
    
    REQUIRE_TRUE(h0Shape == h0CorrectShape, 0, "GRU_CELL operation: wrong shape of previous cell output array, expected is %s, but got %s instead !", h0CorrectShape.c_str(), h0Shape.c_str());
    REQUIRE_TRUE(wxShape == wxCorrectShape, 0, "GRU_CELL operation: wrong shape of input-to-hidden weights array, expected is %s, but got %s instead !", wxCorrectShape.c_str(), wxShape.c_str());
    REQUIRE_TRUE(whShape == whCorrectShape, 0, "GRU_CELL operation: wrong shape of hidden-to-hidden weights array, expected is %s, but got %s instead !", whCorrectShape.c_str(), whShape.c_str());
    REQUIRE_TRUE(bShape  == bCorrectShape,  0, "GRU_CELL operation: wrong shape of biases  array, expected is %s, but got %s instead !", bCorrectShape.c_str(), bShape.c_str());


    helpers::gruCell({x, h0, Wx, Wh, b}, h);

    return Status::OK();
}
DECLARE_SYN(gruCell, gru_cell);


DECLARE_SHAPE_FN(gru_cell) {    
    
    const Nd4jLong* xShapeInfo  = inputShape->at(0);                     // input [bS x inSize]
    const Nd4jLong* h0ShapeInfo = inputShape->at(1);                     // previous cell output [bS x numUnits],  that is at previous time step t-1
    const Nd4jLong* WxShapeInfo = inputShape->at(2);                     // input-to-hidden weights, [inSize   x 3*numUnits]
    const Nd4jLong* WhShapeInfo = inputShape->at(3);                     // hidden-to-hidden weights, [numUnits x 3*numUnits]
    const Nd4jLong* bShapeInfo  = inputShape->at(4);                     // biases, [3*numUnits]

    const int rank     = xShapeInfo[0];              // = 2
    const int bS       = xShapeInfo[1];
    const int inSize   = xShapeInfo[2];
    const int numUnits = h0ShapeInfo[2];

    const std::string h0Shape        = ShapeUtils<T>::shapeAsString(h0ShapeInfo);
    const std::string h0CorrectShape = ShapeUtils<T>::shapeAsString({bS, numUnits});
    const std::string wxShape        = ShapeUtils<T>::shapeAsString(WxShapeInfo);
    const std::string wxCorrectShape = ShapeUtils<T>::shapeAsString({inSize, 3*numUnits});
    const std::string whShape        = ShapeUtils<T>::shapeAsString(WhShapeInfo);
    const std::string whCorrectShape = ShapeUtils<T>::shapeAsString({numUnits, 3*numUnits});
    const std::string bShape         = ShapeUtils<T>::shapeAsString(bShapeInfo);
    const std::string bCorrectShape  = ShapeUtils<T>::shapeAsString({3*numUnits});

    REQUIRE_TRUE(h0Shape == h0CorrectShape, 0, "GRUCELL operation: wrong shape of previous cell output array, expected is %s, but got %s instead !", h0CorrectShape.c_str(), h0Shape.c_str());
    REQUIRE_TRUE(wxShape == wxCorrectShape, 0, "GRUCELL operation: wrong shape of input-to-hidden weights array, expected is %s, but got %s instead !", wxCorrectShape.c_str(), wxShape.c_str());
    REQUIRE_TRUE(whShape == whCorrectShape, 0, "GRUCELL operation: wrong shape of hidden-to-hidden weights array, expected is %s, but got %s instead !", whCorrectShape.c_str(), whShape.c_str());
    REQUIRE_TRUE(bShape  == bCorrectShape,  0, "GRUCELL operation: wrong shape of biases  array, expected is %s, but got %s instead !", bCorrectShape.c_str(), bShape.c_str());
    Nd4jLong *hShapeInfo(nullptr);
    ALLOCATE(hShapeInfo, block.getWorkspace(), shape::shapeInfoLength(rank), Nd4jLong);// [bS x numUnits]

    hShapeInfo[0] = rank;
    hShapeInfo[1] = bS;
    hShapeInfo[2] = numUnits;

    shape::updateStrides(hShapeInfo, shape::order(const_cast<Nd4jLong*>(h0ShapeInfo)));
    return SHAPELIST(hShapeInfo);

}


//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(gru_cell_bp, 6, 5, false, 0, 0) {

    NDArray<T>* x      = INPUT_VARIABLE(0);                                 // input [bS x iS]
    NDArray<T>* h0     = INPUT_VARIABLE(1);                                 // previous cell output [bS x nU]     
    NDArray<T>* Wx     = INPUT_VARIABLE(2);                                 // input-to-hidden  weights, [iS x 3*nU] 
    NDArray<T>* Wh     = INPUT_VARIABLE(3);                                 // hidden-to-hidden weights, [nU x 3*nU] 
    NDArray<T>* b      = INPUT_VARIABLE(4);                                 // biases, [3*nU] 
    NDArray<T>* dLdh   = INPUT_VARIABLE(5);                                 // gradient wrt output, [bS,nU], that is epsilon_next
    NDArray<T> *dLdWx0 = block.width() > 6 ? INPUT_VARIABLE(6) : nullptr;   // gradient wrt Wx at previous time step, [iS, 3*nU]
    NDArray<T> *dLdWh0 = block.width() > 7 ? INPUT_VARIABLE(7) : nullptr;   // gradient wrt Wh at previous time step, [nU, 3*nU]
    NDArray<T> *dLdb0  = block.width() > 8 ? INPUT_VARIABLE(8) : nullptr;   // gradient wrt b at previous time step,  [3*nU]
    
    NDArray<T>* dLdx   = OUTPUT_VARIABLE(0);                                // gradient wrt x,  [bS, iS], that is epsilon
    NDArray<T>* dLdh0  = OUTPUT_VARIABLE(1);                                // gradient wrt h0, [bS, nU]
    NDArray<T>* dLdWx  = OUTPUT_VARIABLE(2);                                // gradient wrt Wx, [iS, 3*nU]
    NDArray<T>* dLdWh  = OUTPUT_VARIABLE(3);                                // gradient wrt Wh, [nU, 3*nU]
    NDArray<T>* dLdb   = OUTPUT_VARIABLE(4);                                // gradient wrt biases,  [3*nU]

    const int rank     = x->rankOf();                               // = 2
    const Nd4jLong bS  = x->sizeAt(0);
    const Nd4jLong iS  = x->sizeAt(1);
    const Nd4jLong nU  = h0->sizeAt(1);    

    const std::string h0Shape          = ShapeUtils<T>::shapeAsString(h0); 
    const std::string h0CorrectShape   = ShapeUtils<T>::shapeAsString({bS, nU});
    const std::string wxShape          = ShapeUtils<T>::shapeAsString(Wx); 
    const std::string wxCorrectShape   = ShapeUtils<T>::shapeAsString({iS, 3*nU}); 
    const std::string whShape          = ShapeUtils<T>::shapeAsString(Wh); 
    const std::string whCorrectShape   = ShapeUtils<T>::shapeAsString({nU, 3*nU}); 
    const std::string bShape           = ShapeUtils<T>::shapeAsString(b); 
    const std::string bCorrectShape    = ShapeUtils<T>::shapeAsString({3*nU});    
    const std::string dLdhShape        = ShapeUtils<T>::shapeAsString(dLdh);
    const std::string dLdhCorrectShape = ShapeUtils<T>::shapeAsString({bS, nU});
    
    REQUIRE_TRUE(h0Shape   == h0CorrectShape,    0, "GRU_CELL_BP op: wrong shape of previous cell output array, expected is %s, but got %s instead !", h0CorrectShape.c_str(), h0Shape.c_str()); 
    REQUIRE_TRUE(wxShape   == wxCorrectShape,    0, "GRU_CELL_BP op: wrong shape of input-to-hidden weights array, expected is %s, but got %s instead !", wxCorrectShape.c_str(), wxShape.c_str()); 
    REQUIRE_TRUE(whShape   == whCorrectShape,    0, "GRU_CELL_BP op: wrong shape of hidden-to-hidden weights array, expected is %s, but got %s instead !", whCorrectShape.c_str(), whShape.c_str());     
    REQUIRE_TRUE(bShape    == bCorrectShape,     0, "GRU_CELL_BP op: wrong shape of biases array, expected is %s, but got %s instead !", bCorrectShape.c_str(), bShape.c_str());     
    REQUIRE_TRUE(dLdhShape == dLdhCorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdh array (epsilon_next), expected is %s, but got %s instead !", dLdhCorrectShape.c_str(), dLdhShape.c_str());     

    if(dLdWx0 != nullptr) {
        const std::string dLdWx0Shape        = ShapeUtils<T>::shapeAsString(dLdWx0);
        const std::string dLdWx0CorrectShape = ShapeUtils<T>::shapeAsString({iS, 3*nU});
        REQUIRE_TRUE(dLdWx0Shape == dLdWx0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdWx0 array (gradient wrt Wx at previous time step), expected is %s, but got %s instead !", dLdWx0CorrectShape.c_str(), dLdWx0Shape.c_str());
    }

    if(dLdWh0 != nullptr) {
        const std::string dLdWh0Shape        = ShapeUtils<T>::shapeAsString(dLdWh0);
        const std::string dLdWh0CorrectShape = ShapeUtils<T>::shapeAsString({nU, 3*nU});
        REQUIRE_TRUE(dLdWh0Shape == dLdWh0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdWh0 array (gradient wrt Wh at previous time step), expected is %s, but got %s instead !", dLdWh0CorrectShape.c_str(), dLdWh0Shape.c_str());
    }

    if(dLdb0 != nullptr) {
        const std::string dLdb0Shape        = ShapeUtils<T>::shapeAsString(dLdb0);
        const std::string dLdb0CorrectShape = ShapeUtils<T>::shapeAsString({3*nU});
        REQUIRE_TRUE(dLdb0Shape == dLdb0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdb0 array (gradient wrt biases at previous time step), expected is %s, but got %s instead !", dLdb0CorrectShape.c_str(), dLdb0Shape.c_str());
    }

    const std::vector<NDArray<T>*> inArrs  = {x, h0, Wx, Wh, b, dLdh, dLdWx0, dLdWh0, dLdb0};
    const std::vector<NDArray<T>*> outArrs = {dLdx, dLdh0, dLdWx, dLdWh, dLdb};

    helpers::gruCellBP<T>(inArrs, outArrs);
    
    return Status::OK();
}


DECLARE_SHAPE_FN(gru_cell_bp) {

    Nd4jLong* xShapeInfo      = inputShape->at(0);                                              // [bS x iS]
    Nd4jLong* h0ShapeInfo     = inputShape->at(1);                                              // [bS x nU]
    Nd4jLong* wxShapeInfo     = inputShape->at(2);                                              // [iS x 3*nU]
    Nd4jLong* whShapeInfo     = inputShape->at(3);                                              // [nU x 3*nU]
    Nd4jLong* bShapeInfo      = inputShape->at(4);                                              // [3*nU]
    Nd4jLong* dLdhShapeInfo   = inputShape->at(5);                                              // [bS x nU]
    
    const int rank    = xShapeInfo[0];                               // = 2
    const Nd4jLong bS = xShapeInfo[1];
    const Nd4jLong iS = xShapeInfo[2];
    const Nd4jLong nU = h0ShapeInfo[2];    

    const std::string h0Shape          = ShapeUtils<T>::shapeAsString(h0ShapeInfo); 
    const std::string h0CorrectShape   = ShapeUtils<T>::shapeAsString({bS, nU});
    const std::string wxShape          = ShapeUtils<T>::shapeAsString(wxShapeInfo); 
    const std::string wxCorrectShape   = ShapeUtils<T>::shapeAsString({iS, 3*nU}); 
    const std::string whShape          = ShapeUtils<T>::shapeAsString(whShapeInfo); 
    const std::string whCorrectShape   = ShapeUtils<T>::shapeAsString({nU, 3*nU}); 
    const std::string bShape           = ShapeUtils<T>::shapeAsString(bShapeInfo); 
    const std::string bCorrectShape    = ShapeUtils<T>::shapeAsString({3*nU});    
    const std::string dLdhShape        = ShapeUtils<T>::shapeAsString(dLdhShapeInfo);
    const std::string dLdhCorrectShape = ShapeUtils<T>::shapeAsString({bS, nU});
    
    REQUIRE_TRUE(h0Shape   == h0CorrectShape,    0, "GRU_CELL_BP op: wrong shape of previous cell output array, expected is %s, but got %s instead !", h0CorrectShape.c_str(), h0Shape.c_str()); 
    REQUIRE_TRUE(wxShape   == wxCorrectShape,    0, "GRU_CELL_BP op: wrong shape of input-to-hidden weights array, expected is %s, but got %s instead !", wxCorrectShape.c_str(), wxShape.c_str()); 
    REQUIRE_TRUE(whShape   == whCorrectShape,    0, "GRU_CELL_BP op: wrong shape of hidden-to-hidden weights array, expected is %s, but got %s instead !", whCorrectShape.c_str(), whShape.c_str());     
    REQUIRE_TRUE(bShape    == bCorrectShape,     0, "GRU_CELL_BP op: wrong shape of biases array, expected is %s, but got %s instead !", bCorrectShape.c_str(), bShape.c_str());     
    REQUIRE_TRUE(dLdhShape == dLdhCorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdh array (epsilon_next), expected is %s, but got %s instead !", dLdhCorrectShape.c_str(), dLdhShape.c_str());     

    if(block.width() > 6) {
        Nd4jLong* dLdWx0ShapeInfo = inputShape->at(6);                                              // [iS x 3*nU]
        const std::string dLdWx0Shape        = ShapeUtils<T>::shapeAsString(dLdWx0ShapeInfo);
        const std::string dLdWx0CorrectShape = ShapeUtils<T>::shapeAsString({iS, 3*nU});    
        REQUIRE_TRUE(dLdWx0Shape == dLdWx0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdWx0 array (gradient wrt Wx at previous time step), expected is %s, but got %s instead !", dLdWx0CorrectShape.c_str(), dLdWx0Shape.c_str());
    }

    if(block.width() > 7) {
        Nd4jLong* dLdWh0ShapeInfo = inputShape->at(7);                                              // [nU x 3*nU]
        const std::string dLdWh0Shape        = ShapeUtils<T>::shapeAsString(dLdWh0ShapeInfo);
        const std::string dLdWh0CorrectShape = ShapeUtils<T>::shapeAsString({nU, 3*nU});
        REQUIRE_TRUE(dLdWh0Shape == dLdWh0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdWh0 array (gradient wrt Wh at previous time step), expected is %s, but got %s instead !", dLdWh0CorrectShape.c_str(), dLdWh0Shape.c_str());
    }

    if(block.width() > 8) {
        Nd4jLong* dLdb0ShapeInfo  = inputShape->at(8);                                              // [3*nU]
        const std::string dLdb0Shape        = ShapeUtils<T>::shapeAsString(dLdb0ShapeInfo);
        const std::string dLdb0CorrectShape = ShapeUtils<T>::shapeAsString({3*nU});
        REQUIRE_TRUE(dLdb0Shape == dLdb0CorrectShape,  0, "GRU_CELL_BP op: wrong shape of dLdb0 array (gradient wrt biases at previous time step), expected is %s, but got %s instead !", dLdb0CorrectShape.c_str(), dLdb0Shape.c_str());
    }
    
    Nd4jLong *dLdxShapeInfo = nullptr;
    COPY_SHAPE(xShapeInfo, dLdxShapeInfo);
    
    Nd4jLong *dLdh0ShapeInfo = nullptr;
    COPY_SHAPE(h0ShapeInfo, dLdh0ShapeInfo);

    Nd4jLong *dLdWxShapeInfo = nullptr;
    COPY_SHAPE(wxShapeInfo, dLdWxShapeInfo);

    Nd4jLong *dLdWhShapeInfo = nullptr;
    COPY_SHAPE(whShapeInfo, dLdWhShapeInfo);

    Nd4jLong *dLdbShapeInfo = nullptr;
    COPY_SHAPE(bShapeInfo, dLdbShapeInfo);

    return SHAPELIST(dLdxShapeInfo, dLdh0ShapeInfo, dLdWxShapeInfo, dLdWhShapeInfo, dLdbShapeInfo);
    
}



}
}


#endif