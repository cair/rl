import gym

if __name__ == "__main__":

    env = gym.make("Acrobot-v1")

    env.reset()


    while 1:
        env.step(0)