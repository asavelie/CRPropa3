#include "mpc/Candidate.h"
#include "mpc/DeflectionCK.h"
#include "mpc/BreakCondition.h"
#include "mpc/ModuleChain.h"
#include "mpc/GlutDisplay.h"
#include "mpc/ParticleState.h"
#include "mpc/SharedPointer.h"
#include "mpc/TurbulentMagneticField.h"
#include "mpc/Units.h"
#include "mpc/Output.h"
#include <iostream>

using namespace mpc;

int main() {
	ModuleChain chain;

	//	HomogeneousMagneticField field(Vector3(0., 0., 1e-13));
	TurbulentMagneticField field(Vector3(0, 0, 0) * Mpc, 64, 100 * kpc, 1. * nG,
			-11. / 3., 2., 8.);
	std::cout << "initializing turbulent field" << std::endl;
	field.initialize();

	chain.add(Priority::Integration,
			new DeflectionCK(&field, DeflectionCK::WorstOffender, 5e-5));

	chain.add(mpc::Priority::AfterIntegration,
			new MaximumTrajectoryLength(100 * Mpc));

//	chain.add(Priority::AfterCommit, new GlutDisplay());
	chain.add(Priority::AfterCommit, new CandidateOutput());

	chain.print();

	ParticleState initial;
//	initial.setPosition(Vector3(-1.08, 0., 0.) * Mpc);
	initial.setPosition(Vector3(2, 2, 2) * Mpc);
	initial.setChargeNumber(1);
	initial.setMass(1 * amu);
	initial.setDirection(Vector3(1., 1., 1.));
	initial.setEnergy(10 * EeV);

	Candidate candidate;
	candidate.current = initial;
	candidate.initial = initial;
	candidate.setNextStep(0.05 * Mpc);

	chain.apply(candidate);

	return 0;
}
