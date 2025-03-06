import numpy as np
from tf_agents.environments import py_environment
from tf_agents.trajectories import time_step as ts
from tf_agents.specs import array_spec

class ShooterPyEnv(py_environment.PyEnvironment):
    """
    Minimal environment so tf_agents can track specs.
    The real logic is in C++. We just store an observation.
    """
    def __init__(self):
        super().__init__()

        # Observation: 11 floats
        self._observation_spec = array_spec.BoundedArraySpec(
            shape=(11,), dtype=np.float32, minimum=-9999.0, maximum=9999.0, name='observation'
        )

        # Flatten multi-discrete (5*3*2=30) -> single discrete: 0..29
        self._action_spec = array_spec.BoundedArraySpec(
            shape=(), dtype=np.int32, minimum=0, maximum=29, name='action'
        )

        self._episode_ended = False
        self._current_observation = np.zeros((11,), dtype=np.float32)

    def action_spec(self):
        return self._action_spec

    def observation_spec(self):
        return self._observation_spec

    def _reset(self):
        self._episode_ended = False
        self._current_observation = np.zeros((11,), dtype=np.float32)
        return ts.restart(self._current_observation)

    def _step(self, action):
        if self._episode_ended:
            return self.reset()
        # Minimal stepping; real logic in C++
        return ts.transition(self._current_observation, reward=0.0, discount=1.0)

    def set_observation(self, obs: np.ndarray):
        self._current_observation = obs.astype(np.float32)

    def end_episode(self, reward=0.0):
        self._episode_ended = True
        return ts.termination(self._current_observation, reward)