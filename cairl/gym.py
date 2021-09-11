from cairlpp.gym import envs

_registry = {
    "CartPole-v0": envs.classic_control.CartPoleEnv,
    "FlashRL-Hearts-v0": envs.flashrl.FlashEnv("https://flashrl.sysx.no/357085_heartsNoAccount.swf")
}


def make(register_id):

    if register_id not in _registry:
        print(f"Unregistered environment with id={register_id}")

    return _registry[register_id]()
