import time


class Ticker:
    def __init__(self):
        self.previous_time = time.time()
        self.frequency = 0
        self.alpha = .5

        self.print_cycle = 5
        self.count = 0

    def tick(self,quite=False):
        """Keeps track of method frequency and returns True if
            this was a print cycle."""
        self.count += 1

        now = time.time()
        diff = now-self.previous_time
        self.previous_time = now

        new_frequency = 1.0/diff
        self.frequency = self.alpha*new_frequency + (1-self.alpha)*self.frequency

        if self.count % self.print_cycle == 0:
            if not quite:
                print "Frequency",self.frequency,"Hz"
            return True

        return False
