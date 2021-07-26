import time
import numpy as np
import gym
from cairl.gym import classic_control
import cv2
def cartpole_runner(env, num_trials):
   step_list = []
   for i in range(num_trials):
      terminal = False
      state = env.reset()
      steps = 0
      while not terminal:
         state, reward, terminal, info = env.step(env.action_space.sample())
         image = env.render("rgb_array")
         #cv2.imshow("lol", image)
         #cv2.waitKey(1)
         steps += 1
      step_list.append(steps)
   return sum(step_list) / num_trials

def validate_env(env_true, env_test):

   true_state = env_true.reset()
   env_test.__override_state(*true_state)

   print(env_true.state, env_test.state)

   for i in range(60):
      a = env_true.action_space.sample()
      true_data = env_true.step(a)
      test_data = env_test.step(a)
      true_state = true_data[0]
      test_state = test_data[0]

      # Test state difference
      state_diff = np.asarray(true_state) - np.asarray(test_state)
      state_diffsum = np.sum(state_diff)

      assert state_diffsum >= 0.0, f"{state_diff}, {state_diffsum}"

      # Test terminal difference
      terminal_diff = np.asarray([int(true_data[2])]) - np.asarray([int(true_data[2])])
      terminal_diffsum = np.sum(terminal_diff)
      assert terminal_diffsum >= 0.0


if __name__ == "__main__":

   runs = 1000
   cpp_env = classic_control.CartPoleEnv()
   py_env = gym.envs.make("CartPole-v0")

   validate_env(env_true=py_env, env_test=cpp_env)


   s = time.time()
   print(f"Runs: {runs}, Average-Steps: {cartpole_runner(cpp_env, runs)}, Time-Used: {(time.time() - s) * 1000.0}ms")
   s = time.time()

   print(f"Runs: {runs}, Average-Steps: {cartpole_runner(py_env, runs)}, Time-Used: {(time.time() - s) * 1000.0}ms")
