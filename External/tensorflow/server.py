# server.py

from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn
import numpy as np
import tensorflow as tf

from shooter_env import ShooterPyEnv
from agent_setup import create_dqn_agent, create_replay_buffer, train_one_step
from tf_agents.trajectories import trajectory
from tf_agents.trajectories import time_step as ts

app = FastAPI()

# ---------- Create TWO Environments ----------
env_player1 = ShooterPyEnv()
env_player2 = ShooterPyEnv()

# ---------- Create TWO Agents ----------
agent_player1 = create_dqn_agent(env_player1, learning_rate=1e-3)
agent_player2 = create_dqn_agent(env_player2, learning_rate=1e-3)

# ---------- Create TWO Replay Buffers ----------
replay_buffer_player1 = create_replay_buffer(agent_player1)
replay_buffer_player2 = create_replay_buffer(agent_player2)

# ---------- Track time_steps + last actions separately ----------
current_time_step_player1 = env_player1.reset()
last_action_player1 = None

current_time_step_player2 = env_player2.reset()
last_action_player2 = None

class StateRequest(BaseModel):
    """
    Matches your C++ RLRequest structure.
    """
    player_x: float
    player_z: float
    move_speed: float
    rot_x: float
    rot_speed: float
    player_health: float
    attack_cooldown: float
    opp_x: float
    opp_z: float
    opp_rot: float
    opp_health: float

    reward: float = 0.0
    done: bool = False

@app.post("/act_player1")
def get_action_player1(state_req: StateRequest):
    global current_time_step_player1, last_action_player1

    # 1) Convert input to a numpy observation
    obs = np.array([
        state_req.player_x,
        state_req.player_z,
        state_req.move_speed,
        state_req.rot_x,
        state_req.rot_speed,
        state_req.player_health,
        state_req.attack_cooldown,
        state_req.opp_x,
        state_req.opp_z,
        state_req.opp_rot,
        state_req.opp_health
    ], dtype=np.float32)

    # 2) If episode ended
    if state_req.done:
        terminal_ts = env_player1.end_episode(reward=state_req.reward)
        if last_action_player1 is not None:
            final_traj = trajectory.from_transition(current_time_step_player1, last_action_player1, terminal_ts)
            replay_buffer_player1.add_batch(final_traj)
        current_time_step_player1 = env_player1.reset()
        last_action_player1 = None
        return {"action": 0, "msg": "P1 episode ended. No new action."}

    # 3) Update environment with new observation
    env_player1.set_observation(obs)

    # 4) If we have a last action, store the transition for that action
    if last_action_player1 is not None:
        reward_ts = ts.transition(env_player1._current_observation, reward=state_req.reward, discount=1.0)
        traj = trajectory.from_transition(current_time_step_player1, last_action_player1, reward_ts)
        replay_buffer_player1.add_batch(traj)
        current_time_step_player1 = reward_ts

    # 5) Query the agent for an action
    tf_obs = tf.convert_to_tensor([obs], dtype=tf.float32)
    policy_step_p1 = agent_player1.policy.action(ts.restart(tf_obs))
    action_p1 = policy_step_p1.action.numpy()[0]  # single int in [0..29]

    # 6) "Step" environment to produce next_time_step
    next_time_step_player1 = env_player1._step(action_p1)
    last_action_player1 = policy_step_p1
    current_time_step_player1 = next_time_step_player1

    return {"action": int(action_p1), "msg": "OK"}

@app.post("/act_player2")
def get_action_player2(state_req: StateRequest):
    global current_time_step_player2, last_action_player2

    # 1) Convert input to a numpy observation
    obs = np.array([
        state_req.player_x,
        state_req.player_z,
        state_req.move_speed,
        state_req.rot_x,
        state_req.rot_speed,
        state_req.player_health,
        state_req.attack_cooldown,
        state_req.opp_x,
        state_req.opp_z,
        state_req.opp_rot,
        state_req.opp_health
    ], dtype=np.float32)

    # 2) If episode ended
    if state_req.done:
        terminal_ts = env_player2.end_episode(reward=state_req.reward)
        if last_action_player2 is not None:
            final_traj = trajectory.from_transition(current_time_step_player2, last_action_player2, terminal_ts)
            replay_buffer_player2.add_batch(final_traj)
        current_time_step_player2 = env_player2.reset()
        last_action_player2 = None
        return {"action": 0, "msg": "P2 episode ended. No new action."}

    # 3) Update environment with new observation
    env_player2.set_observation(obs)

    # 4) If we have a last action, store the transition for that action
    if last_action_player2 is not None:
        reward_ts = ts.transition(env_player2._current_observation, reward=state_req.reward, discount=1.0)
        traj = trajectory.from_transition(current_time_step_player2, last_action_player2, reward_ts)
        replay_buffer_player2.add_batch(traj)
        current_time_step_player2 = reward_ts

    # 5) Query the agent for an action
    tf_obs = tf.convert_to_tensor([obs], dtype=tf.float32)
    policy_step_p2 = agent_player2.policy.action(ts.restart(tf_obs))
    action_p2 = policy_step_p2.action.numpy()[0]  # single int in [0..29]

    # 6) "Step" environment to produce next_time_step
    next_time_step_player2 = env_player2._step(action_p2)
    last_action_player2 = policy_step_p2
    current_time_step_player2 = next_time_step_player2

    return {"action": int(action_p2), "msg": "OK"}

@app.post("/train_player1")
def train_batch_player1():
    loss_info = train_one_step(agent_player1, replay_buffer_player1, batch_size=32)
    return {
        "loss": float(loss_info.loss),
        "msg": "Trained one minibatch for Player1."
    }

@app.post("/train_player2")
def train_batch_player2():
    loss_info = train_one_step(agent_player2, replay_buffer_player2, batch_size=32)
    return {
        "loss": float(loss_info.loss),
        "msg": "Trained one minibatch for Player2."
    }

if __name__ == "__main__":
    # Remember to install uvicorn: `pip install uvicorn`
    uvicorn.run("server:app", host="0.0.0.0", port=8000, reload=False)