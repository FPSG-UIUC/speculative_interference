#!/usr/bin/env bash

#####
# Script to set up the host with a confiuration that is useful for side channel experiments
# NOTE: to reset these values, reboot the machine.
#####

# Stop unnecessary services
sudo service aesmd stop
sudo service auditd stop
sudo service iscsid stop
sudo systemctl stop syslog.socket rsyslog.service
sudo systemctl stop polkitd

# Load MSR
sudo modprobe msr

# Enable prefetchers, if prefetchers help
# Alternatively, write 15 to disable them
# This works on Nehalem, Westmere, Sandy Bridge, Ivy Bridge, Haswell, Broadwell, Skylake, Coffeelake
for i in {0..7}
do
    sudo wrmsr -p $i 0x1a4 15
    echo $i
done

# Enable some hugepages
echo 1024 | sudo tee /proc/sys/vm/nr_hugepages

# Allow usage of performance counters
echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid

# Disable ASLR
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

# Disable transparent huge pages
echo "never" | sudo tee /sys/kernel/mm/transparent_hugepage/enabled

# Fix governor to performance
echo "performance" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Make malloc always succeed
sudo sysctl -q -w vm.overcommit_memory=1