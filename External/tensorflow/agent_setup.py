import tensorflow as tf
import numpy as np

from tf_agents.agents.dqn import dqn_agent
from tf_agents.networks import q_network
from tf_agents.utils import common
from tf_agents.replay_buffers import tf_uniform_replay_buffer
from tf_agents.trajectories import trajectory

def create_dqn_agent(env, learning_rate=1e-3):
    """
    Create a simple DQN agent with a QNetwork
    that takes 11-dim observation and outputs Q-values for 30 discrete actions.
    """
    q_net = q_network.QNetwork(
        input_tensor_spec=env.observation_spec(),
        action_spec=env.action_spec(),
        fc_layer_params=(128, 128)
    )
    
    optimizer = tf.keras.optimizers.Adam(learning_rate=learning_rate)
    global_step = tf.compat.v1.train.get_or_create_global_step()
    
    agent = dqn_agent.DqnAgent(
        time_step_spec=env.time_step_spec(),
        action_spec=env.action_spec(),
        q_network=q_net,
        optimizer=optimizer,
        epsilon_greedy=0.1,  # can decay it over time if needed
        gamma=0.99,
        train_step_counter=global_step
    )
    
    agent.initialize()
    return agent

def create_replay_buffer(agent, max_length=100000):
    """
    Create a uniform replay buffer for storing transitions.
    """
    replay_buffer = tf_uniform_replay_buffer.TFUniformReplayBuffer(
        data_spec=agent.collect_data_spec,
        batch_size=1,  # single env
        max_length=max_length
    )
    return replay_buffer

@tf.function
def train_one_step(agent, replay_buffer, batch_size=32):
    """
    Sample a batch of experience from the replay buffer and train.
    """
    dataset = replay_buffer.as_dataset(
        sample_batch_size=batch_size, 
        num_steps=2, 
        single_deterministic_pass=False
    )
    iterator = iter(dataset)
    experience, _ = next(iterator)
    return agent.train(experience)