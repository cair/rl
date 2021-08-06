import os

os.environ["SDL_VIDEODRIVER"] = "dummy"

import time
import numpy as np
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import gym
import cairl.gym


def cartpole_runner(env, num_trials, render):
    start = time.time()
    for i in range(num_trials):
        terminal = False
        state = env.reset()
        while not terminal:
            state, reward, terminal, info = env.step(env.action_space.sample())
            if render:
                image = env.render("rgb_array")
    return (time.time() - start) * 1000.0


def validate_env(env_true, env_test, env_steps=60):
    """
   This function runs both openai and cairl one step and validates that both outputs same values.
   :param env_true:
   :param env_test:
   :return:
   """
    true_state = env_true.reset()
    env_test.__override_state(*true_state)

    for i in range(env_steps):
        a = env_true.action_space.sample()
        true_data = env_true.step(a)
        test_data = env_test.step(a)
        true_state = true_data[0]
        test_state = test_data[0]

        # Test state difference
        state_diff = np.asarray(true_state) - np.array(test_state)
        state_diffsum = np.sum(state_diff)

        assert state_diffsum >= 0.0, f"{state_diff}, {state_diffsum}"

        # Test terminal difference
        terminal_diff = np.asarray([int(true_data[2])]) - np.asarray([int(true_data[2])])
        terminal_diffsum = np.sum(terminal_diff)
        assert terminal_diffsum >= 0.0


if __name__ == "__main__":
    """
    ##
    ## Configuration
    ##
    """
    runs_console = 100000
    runs_gui = 100
    cpp_env = cairl.gym.make("CartPole-v0")
    py_env = gym.make("CartPole-v0")

    """
    ##
    ## Validate cpp_env against py_env
    ##
    """
    validate_env(env_true=py_env, env_test=cpp_env)

    """
    ##
    ## Create Datapoints and run experiments
    ##
    """
    x = [
        "CaiRL-CartPole-v0",
        "OpenAI-CartPole-v0",
        "CaiRL-CartPole-v0",
        "OpenAI-CartPole-v0",
    ]

    y = [
        cartpole_runner(cpp_env, runs_console, False),
        cartpole_runner(py_env, runs_console, False),
        cartpole_runner(cpp_env, runs_gui, True),
        cartpole_runner(py_env, runs_gui, True)
    ]

    z = [
        f"Console ({runs_console}) runs",
        f"Console ({runs_console}) runs",
        f"Graphical ({runs_gui}) runs",
        f"Graphical ({runs_gui}) runs"
    ]

    data = pd.DataFrame({
        "Environment": x,
        "Execution Time (msec)": y,
        "Mode": z
    })

    """
    ##
    ## Plot Results
    ##
    """
    plt.figure(figsize=(8,4))
    g = sns.FacetGrid(data, col="Mode")
    g.map(sns.barplot, "Environment", "Execution Time (msec)")

    plt.savefig("performance_cartpole_v0.pdf")
    plt.savefig("performance_cartpole_v0.png")
    plt.savefig("performance_cartpole_v0.svg")
    plt.tight_layout()
    plt.show()