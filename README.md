# hypervisor-detection
The work consists on using timing strategies to detect the presence of virtualisation. To this end, the goal is to take many samples of the time taken by one or more CPU instructions affected by virtualisation and to find its minimum, which will provide an approximation of the exact time those instructions take to execute. By comparing that time with ground true values obtained in a nonvirtualised environment, we are capable of detecting the presence of virtualisation.
