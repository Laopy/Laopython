from device import Motor
from time import sleep
from twowheel import TwoWheelController


def test_controller(controller):
    for _ in range(2):
        controller.set_axis()
        sleep(1)

        controller.set_axis(x=1)
        sleep(1)

        controller.set_axis(x=-1)
        sleep(1)
      
        controller.set_axis(x=0)
        sleep(1)

        controller.set_axis(y=1)
        sleep(1)

        controller.set_axis(y=-1)
        sleep(1)

        controller.set_axis(y=0)
        sleep(1)


if __name__ == "__main__":
    controller = TwoWheelController()
    test_controller(controller)
