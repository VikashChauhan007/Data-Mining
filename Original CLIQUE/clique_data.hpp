#ifndef CLIQUE_DATA_HPP_INCLUDED
#define CLIQUE_DATA_HPP_INCLUDED


#pragma once


#include <clique_block.hpp>
#include <cluster_data.hpp>


namespace pyclustering {

namespace clst {


/*!

@brief  Sequence container where CLIQUE blocks are stored.

*/
using clique_block_sequence = std::vector<clique_block>;


/*!

@class  clique_data clique_data.hpp pyclustering/cluster/clique_data.hpp

@brief  A storage where CLIQUE clustering results are stored.

*/
class clique_data : public cluster_data {
private:
    clique_block_sequence   m_blocks;
    clst::noise             m_noise;

public:
    /*!

    @brief  Returns constant reference to CLIQUE blocks that are formed during clustering process.

    */
    const clique_block_sequence & blocks() const { return m_blocks; }

    /*!

    @brief  Returns reference to CLIQUE blocks that are formed during clustering process.

    */
    clique_block_sequence & blocks() { return m_blocks; }

    /*!

    @brief  Returns constant reference to outliers that are allocated during clustering process.

    */
    const clst::noise & noise() const { return m_noise; }

    /*!

    @brief  Returns reference to outliers that are allocated during clustering process.

    */
    clst::noise & noise() { return m_noise; }
};


}

}


#endif // CLIQUE_DATA_HPP_INCLUDED
