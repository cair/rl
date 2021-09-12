from cairlpp.gym import envs


class Registry:

    registry = {
        "DeepRTSOneVersusOne-v0": envs.deeprts.DeepRTSOneVersusOne,
        "DeepRTSLavaMaze-v0": envs.deeprts.DeepRTSLavaMaze,
        "DeepRTSGoldCollectFifteen-v0": envs.deeprts.DeepRTSGoldCollectFifteen,
        "Acrobot-v3": envs.classic_control.AcrobotEnv,
        "MountainCarContinuous-v3": envs.classic_control.MountainCarContinuousEnv,
        "CartPole-v3": envs.classic_control.CartPoleEnv,
        "MountainCar-v3": envs.classic_control.MountainCarEnv,
        "Pendulum-v3": envs.classic_control.PendulumEnv,
        "FlashRL-Hearts-v0": lambda: envs.flashrl.FlashEnv("FlashRLHearts-v0", "https://flashrl.sysx.no/357085_heartsNoAccount.swf"),
        "FlashRL-Multitask-v2": lambda: envs.flashrl.MultitaskEnv(),
    }

    @staticmethod
    def all():
        return list( Registry.registry.keys())


registry = Registry


def make(register_id):
    if register_id not in Registry.registry:
        print(f"Unregistered environment with id={register_id}")
    return Registry.registry[register_id]()


# if __name__ == "__main__":
#     print(registry.all())
#     print("Running testing")