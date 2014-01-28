#include <nbody/nbody.h>

void Simulation::evolveSystemFor(const double t) {
	for (size_t i = 0; i < _body.size(); i++) {
		Vector3 a = Vector3{0, 0, 0};
		for (size_t j = 0; j < body.size(); j++) {
			if (j != i) {
				a = a + gravity(_body[i], _body[j]);
			}
		}
		_body[i].evolve(a);
	}
}
