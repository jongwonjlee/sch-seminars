#ifndef G2OFACTORY_H
#define G2OFACTORY_H

#include <iostream>
#include "g2oconfig.h"
#include "g2oall.h"

class G2oFactory
{
    typedef g2o::BlockSolverX           BSVar;
    typedef g2o::BlockSolver_6_3        BSSE3;
    typedef g2o::BlockSolver_7_3        BSSim3;
    typedef g2o::BlockSolver_3_2        BSSE2;

    typedef g2o::OptimizationAlgorithmLevenberg     Levenberg;
    typedef g2o::OptimizationAlgorithmGaussNewton   GaussNewton;

public:
    static g2o::SparseOptimizer* optimizerFactory(G2oConfig& options)
    {
        g2o::OptimizationAlgorithm* algorithm;
        if(options.block_type == BlockType::VAR)
        {
            auto block_solver = solverFactory<BSVar,
                                              BSVar::LinearSolverType,
                                              BSVar::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::SE3)
        {
            auto block_solver = solverFactory<BSSE3,
                                              BSSE3::LinearSolverType,
                                              BSSE3::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::Sim3)
        {
            auto block_solver = solverFactory<BSSim3,
                                              BSSim3::LinearSolverType,
                                              BSSim3::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::SE2)
        {
            auto block_solver = solverFactory<BSSE2,
                                              BSSE2::LinearSolverType,
                                              BSSE2::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }

        g2o::SparseOptimizer* optimizer = new g2o::SparseOptimizer;
        optimizer->setAlgorithm(algorithm);
        optimizer->setVerbose(options.verbosity);

        return optimizer;
    }

private:

    template <typename BSType, typename LSType, typename PMType>
    static std::unique_ptr<BSType> solverFactory(SolverType solver_type)
    {
        std::unique_ptr<LSType> linear_solver;
        if(solver_type == SolverType::DENSE)
            linear_solver = g2o::make_unique<g2o::LinearSolverDense<PMType>>();
        else if(solver_type == SolverType::CHOLMOD)
            linear_solver = g2o::make_unique<g2o::LinearSolverCholmod<PMType>>();
        else if(solver_type == SolverType::CSPARSE)
            linear_solver = g2o::make_unique<g2o::LinearSolverCSparse<PMType>>();

        std::unique_ptr<BSType> block_solver;
        block_solver = g2o::make_unique<BSType>(std::move(linear_solver));
        return block_solver;
    }

    template <typename BSType>
    static g2o::OptimizationAlgorithm* algorithmFactory
        (AlgorithmType algo_type, BSType block_solver)
    {
        g2o::OptimizationAlgorithm* algorithm;
        if(algo_type == AlgorithmType::Levenberg)
            algorithm = new Levenberg(std::move(block_solver));
        else if(algo_type == AlgorithmType::GaussNewton)
            algorithm = new GaussNewton(std::move(block_solver));
        return algorithm;
    }

};

#endif // G2OFACTORY_H
