/*
 * Copyright 2016- huaxz <huaxz1986@163.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: huaxz1986@163.com (huaxz)
 */
#ifndef BELLMAN_FORD_TEST
#define BELLMAN_FORD_TEST
#include "../../../google_test/gtest.h"
#include "bellmanford.h"
#include "../../basic_graph/graph_representation/graph/graph.h"
#include "../../basic_graph/graph_representation/graph_vertex/vertexp.h"
using IntroductionToAlgorithm::GraphAlgorithm::Graph;
using IntroductionToAlgorithm::GraphAlgorithm::VertexP;
using IntroductionToAlgorithm::GraphAlgorithm::initialize_single_source;
using IntroductionToAlgorithm::GraphAlgorithm::relax;
using IntroductionToAlgorithm::GraphAlgorithm::bellman_ford;
using IntroductionToAlgorithm::GraphAlgorithm::unlimit;
namespace{
    const int B_NUM=10;  /*!< 图顶点数量*/
}

//!BellmanFordTest:测试类，用于为测试提供基础数据
/*!
*
* `BellmanFordTest`是 `::testing::Test` 的子类。它主要用于为每一个`TEST_F`准备测试环境
*/
class BellmanFordTest:public ::testing::Test
{
public:
    typedef Graph<B_NUM,VertexP<int>> GraphType; /*!< 模板实例化的图类型，该图的顶点类型为`VertexP<int>`*/
    typedef VertexP<int> VertexType; /*!< 图的顶点类型为`VertexP<int>`*/
protected:
    void SetUp()
    {
        _1v_graph=std::make_shared<GraphType>(-1);//边的无效权重为-1
        _1v_graph->add_vertex(0);  //该图只有一个顶点

        _1e_graph=std::make_shared<GraphType>(-1);//边的无效权重为-1
        _1e_graph->add_vertex(0);
        _1e_graph->add_vertex(0);
        _1e_graph->add_edge(std::make_tuple(0,1,1));  //该图只有一条边


        //****  含顶点图和边图：10个顶点，9条边：0-->1-->2....-->9(权重均为1)   ****
        _normal_graph=std::make_shared<GraphType>(-1); //边的无效权重为-1
        for(int i=0;i<B_NUM;i++)
            _normal_graph->add_vertex(0);
        for(int i=0;i<B_NUM-1;i++)
                _normal_graph->add_edge(std::make_tuple(i,i+1,1)); //该图的边是从左到右组成一个链条

        //****  含顶点图和边图：10个顶点，10条边 :0-->1-->2....-->9(权重均为1)以及 9-->8(权重-2)  ****
        _minus_graph=std::make_shared<GraphType>(-1); //边的无效权重为-1
        for(int i=0;i<B_NUM;i++)
            _minus_graph->add_vertex(0);
        for(int i=0;i<B_NUM-1;i++)
                _minus_graph->add_edge(std::make_tuple(i,i+1,1));//链条
        _minus_graph->add_edge(std::make_tuple(9,8,-2)); //最后构成一个负权值回路
    }
    void TearDown(){}

    std::shared_ptr<GraphType> _1v_graph; /*!< 指向一个图，该图只有一个顶点*/
    std::shared_ptr<GraphType> _1e_graph; /*!< 指向一个图，该图只有一条边*/
    std::shared_ptr<GraphType> _normal_graph; /*!< 指向一个图，该图的边组成一条链条*/
    std::shared_ptr<GraphType> _minus_graph; /*!< 指向一个图，该图的边包含负权值回路*/
};

//!BellmanFordTest:测试单源最短路径的的initialize_single_source函数
/*!
* `test_initialize_single_source`:测试单源最短路径的的initialize_single_source函数
*/
TEST_F(BellmanFordTest,test_initialize_single_source)
{
    EXPECT_THROW(initialize_single_source(std::shared_ptr<GraphType>(),0),std::invalid_argument );
    EXPECT_THROW(initialize_single_source(_1v_graph,1),std::invalid_argument );

    initialize_single_source(_normal_graph,0);
    for(int i=0;i<B_NUM;i++)
    {
        EXPECT_FALSE(_normal_graph->vertexes[i]->parent);
        if(i==0) EXPECT_EQ(_normal_graph->vertexes[i]->key,0);
        else EXPECT_EQ(_normal_graph->vertexes[i]->key,unlimit<VertexType::KeyType>());
    }

}
//!BellmanFordTest:测试单源最短路径的的relax函数
/*!
* `test_relax`:测试单源最短路径的的relax函数
*/
TEST_F(BellmanFordTest,test_relax)
{
    initialize_single_source(_normal_graph,0);
    EXPECT_EQ(_normal_graph->vertexes[1]->key,unlimit<VertexType::KeyType>());
    EXPECT_FALSE(_normal_graph->vertexes[1]->parent);

    relax(_normal_graph->vertexes[0],_normal_graph->vertexes[1],_normal_graph->weight(0,1));
    EXPECT_EQ(_normal_graph->vertexes[1]->key,1);
    EXPECT_EQ(_normal_graph->vertexes[1]->parent,_normal_graph->vertexes[0]);
}
//!BellmanFordTest:测试单源最短路径的的bellman_ford算法
/*!
* `test_bellman_ford`:测试单源最短路径的的bellman_ford算法
*/
TEST_F(BellmanFordTest,test_bellman_ford)
{
    //************  只有一个顶点的图  ************
    EXPECT_THROW(bellman_ford(_1v_graph,1),std::invalid_argument);
    EXPECT_TRUE(bellman_ford(_1v_graph,0));
    EXPECT_EQ(_1v_graph->vertexes[0]->key,0);
    //**********  只有一条边的图  ***************
    EXPECT_TRUE(bellman_ford(_1e_graph,0));
    EXPECT_EQ(_1e_graph->vertexes[0]->key,0);
    EXPECT_EQ(_1e_graph->vertexes[1]->key,1);
    EXPECT_EQ(_1e_graph->vertexes[1]->parent,_1e_graph->vertexes[0]);
    //**********  链边的图 ***************
    EXPECT_TRUE(bellman_ford(_normal_graph,0));
    EXPECT_EQ(_normal_graph->vertexes[0]->key,0);
    for(int i=1;i<B_NUM;i++)
    {
        EXPECT_EQ(_normal_graph->vertexes[i]->key,i);
        EXPECT_EQ(_normal_graph->vertexes[i]->parent,_normal_graph->vertexes[i-1]);
    }
    //*********** 含有负权值回路的图  ***********
    EXPECT_FALSE(bellman_ford(_minus_graph,0));
}
#endif // BELLMAN_FORD_TEST

