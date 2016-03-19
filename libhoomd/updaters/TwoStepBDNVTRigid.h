/*
Highly Optimized Object-oriented Many-particle Dynamics -- Blue Edition
(HOOMD-blue) Open Source Software License Copyright 2009-2016 The Regents of
the University of Michigan All rights reserved.

HOOMD-blue may contain modifications ("Contributions") provided, and to which
copyright is held, by various Contributors who have granted The Regents of the
University of Michigan the right to modify and/or distribute such Contributions.

You may redistribute, use, and create derivate works of HOOMD-blue, in source
and binary forms, provided you abide by the following conditions:

* Redistributions of source code must retain the above copyright notice, this
list of conditions, and the following disclaimer both in the code and
prominently in any materials provided with the distribution.

* Redistributions in binary form must reproduce the above copyright notice, this
list of conditions, and the following disclaimer in the documentation and/or
other materials provided with the distribution.

* All publications and presentations based on HOOMD-blue, including any reports
or published results obtained, in whole or in part, with HOOMD-blue, will
acknowledge its use according to the terms posted at the time of submission on:
http://codeblue.umich.edu/hoomd-blue/citations.html

* Any electronic documents citing HOOMD-Blue will link to the HOOMD-Blue website:
http://codeblue.umich.edu/hoomd-blue/

* Apart from the above required attributions, neither the name of the copyright
holder nor the names of HOOMD-blue's contributors may be used to endorse or
promote products derived from this software without specific prior written
permission.

Disclaimer

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND/OR ANY
WARRANTIES THAT THIS SOFTWARE IS FREE OF INFRINGEMENT ARE DISCLAIMED.

IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Maintainer: ndtrung

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

/*! \file TwoStepBDNVTRigid.h
    \brief Declares an updater that implements BD NVT dynamics for rigid bodies
*/

#include "TwoStepNVERigid.h"
#include "Variant.h"
#include "saruprng.h"

#ifndef __TWO_STEP_BD_NVT_RIGID_H__
#define __TWO_STEP_BD_NVT_RIGID_H__


//! Integrates part of the system forward in two steps in the NVE ensemble with Langevin thermostat
/*! Implements velocity-verlet NVE integration through the IntegrationMethodTwoStep interface

 \ingroup updaters
*/
class TwoStepBDNVTRigid : public TwoStepNVERigid
    {
    public:
        //! Constructor
        TwoStepBDNVTRigid(boost::shared_ptr<SystemDefinition> sysdef,
                          boost::shared_ptr<ParticleGroup> group,
                          boost::shared_ptr<Variant> T,
                          unsigned int seed,
                          bool gamma_diam,                                     
                          bool noiseless_t,
                          bool noiseless_r);
        virtual ~TwoStepBDNVTRigid();

        //! Sets gamma for a given particle type
        void setGamma(unsigned int typ, Scalar gamma);
        void setGamma_r(Scalar gamma_r);
        

        //! Performs the second step
        virtual void integrateStepOne(unsigned int timestep);
        virtual void integrateStepTwo(unsigned int timestep);

    protected:
        unsigned int m_seed;              //!< The seed for the RNG of the Stochastic Bath
        bool m_gamma_diam;                //!< flag to enable gamma set to the diameter of each particle

        GPUVector<Scalar> m_gamma;         //!< List of per type gammas to use
        Scalar m_gamma_r;         //!< List of per type gammas to use
        bool m_noiseless_r;         //!< If set true, there will be no random torque in rotational DOF
        bool m_noiseless_t;         //!< If set true, there will be no random force in translational DOF
        
        //! Method to be called when number of types changes
        virtual void slotNumTypesChange();

    private:
        //! Connection to the signal notifying when number of particle types changes
        boost::signals2::connection m_num_type_change_connection;
    };

//! Exports the TwoStepBDNVTRigid class to python
void export_TwoStepBDNVTRigid();

#endif
