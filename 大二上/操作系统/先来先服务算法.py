def fcfs(processes):
    processes.sort(key=lambda x: x['arrival_time'])
    # 用到达时间对进程排序进行处理
    current_time = 0
    # 设置进程处理时间从0开始
    results = []
    # 创建空列表储存进程结果
    for process in processes:
        # 从进程里循环执行，直到所有进程都被执行
        # 检查时间合理性，如果当前时间小于进程到达时间，说明CPU在这段时间是空闲的，将当前时间更新为进程的到达时间
        if current_time < process['arrival_time']:
            current_time = process['arrival_time']
            print(f"CPU空闲，从时间 {process['arrival_time']} 开始执行进程 {process['process_id']}")
        else:
            print(f"在时间 {current_time} 开始执行进程 {process['process_id']}")
        # 计算进程的完成时间 = 当前时间 + 进程的执行时间
        completion_time = current_time + process['burst_time']
        # 计算进程的周转时间 = 完成时间 - 到达时间
        turnaround_time = completion_time - process['arrival_time']
        # 计算进程的等待时间 = 周转时间 - 执行时间
        waiting_time = turnaround_time - process['burst_time']
        # 计算带权周转时间 = 周转时间 / 执行时间
        weighted_turnaround_time = turnaround_time / process['burst_time']
        # 当前时间更新为当前进程的完成时间，为下一个进程做准备
        current_time = completion_time
        print(f"进程 {process['process_id']} 在时间 {completion_time} 完成执行")
        # 将当前进程的调度结果保存到结果列表中
        result = {
            'process_id': process['process_id'],  # 进程ID
            'arrival_time': process['arrival_time'],  # 到达时间
            'burst_time': process['burst_time'],  # 执行时间
            'completion_time': completion_time,  # 完成时间
            'turnaround_time': turnaround_time,  # 周转时间
            'waiting_time': waiting_time,  # 等待时间
            'weighted_turnaround_time': weighted_turnaround_time  # 带权周转时间
        }
        # 让每个result中包含进程ID、到达时间、执行时间等信息
        results.append(result)
        # 把每个进程的result添加到results集合中，此时results=[{result i},{result i+1}......]

    # 步骤11：返回所有进程的调度结果
    return results

def print_results(results):
    """打印调度结果的详细信息"""
    print("\n" + "=" * 80)
    print("先来先服务调度算法详细结果")
    print("=" * 80)
    print("进程ID\t到达时间\t执行时间\t完成时间\t周转时间\t等待时间\t带权周转时间")
    print("-" * 80)

    # 初始化总周转时间、总等待时间和总带权周转时间，用于计算平均值
    total_turnaround = 0
    total_waiting = 0
    total_weighted_turnaround = 0

    # 遍历所有进程的结果，逐个打印
    for result in results:
        print(f"{result['process_id']}\t\t\t{result['arrival_time']}\t\t{result['burst_time']}\t\t"
              f"{result['completion_time']}\t\t{result['turnaround_time']}\t\t"
              f"{result['waiting_time']}\t\t{result['weighted_turnaround_time']:.2f}")

        # 累加周转时间、等待时间和带权周转时间
        total_turnaround += result['turnaround_time']
        total_waiting += result['waiting_time']
        total_weighted_turnaround += result['weighted_turnaround_time']

    print("-" * 80)

    # 计算并打印平均周转时间、平均等待时间和平均带权周转时间
    avg_turnaround = total_turnaround / len(results)
    avg_waiting = total_waiting / len(results)
    avg_weighted_turnaround = total_weighted_turnaround / len(results)


    print(f"平均周转时间: {avg_turnaround:.2f}")
    # .2f保留两位小数
    print(f"平均等待时间: {avg_waiting:.2f}")
    print(f"平均带权周转时间: {avg_weighted_turnaround:.2f}")
    print("=" * 80)


def get_user_input():
    processes = []
    # 创建进程空列表
    print("\n请输入进程信息（输入完成后回车结束）")
    print("格式：进程ID 到达时间 执行时间")
    print("示例：P1 0 5")
    print("-" * 40)

    process_count = 1

    while True:
        try:
            user_input = input(f"进程{process_count} (回车结束输入): ").strip()
            # .strip() 去除字符串开头和结尾的空白字符

            # 如果用户输入空行，结束输入
            if user_input == "":
                if process_count == 1:
                    print("至少需要输入一个进程！")
                    continue
                break

            # 用空格分割输入内容
            parts = user_input.split()

            if len(parts) != 3:
                # 判断分割后用户输入的内容是否为三部分，即ID，到达时间，执行时间
                print("输入格式错误！请按照 '进程ID 到达时间 执行时间' 的格式输入")
                continue

            process_id = parts[0]
            # 将输入的第一部分赋值给进程ID
            arrival_time = int(parts[1])
            # 强制转换第二部分为int类型赋值为到达时间
            burst_time = int(parts[2])
            # 强制转换第三部分为int类型赋值为执行时间

            # 验证输入数据的有效性
            if arrival_time < 0:
                print("到达时间不能为负数！")
                continue
            if burst_time <= 0:
                print("执行时间必须大于0！")
                continue

            processes.append({
                'process_id': process_id,
                'arrival_time': arrival_time,
                'burst_time': burst_time
            })
            # 添加进程到进程集里，相当于等待队列

            process_count += 1
            print(f"已添加进程 {process_id}: 到达时间={arrival_time}, 执行时间={burst_time}")

        except ValueError:
            print("输入格式错误！到达时间和执行时间必须是数字")
        except KeyboardInterrupt:
            print("\n\n输入被中断，使用示例数据")
            return [
                {'process_id': 'P1', 'arrival_time': 0, 'burst_time': 5},
                {'process_id': 'P2', 'arrival_time': 1, 'burst_time': 3},
                {'process_id': 'P3', 'arrival_time': 2, 'burst_time': 8},
                {'process_id': 'P4', 'arrival_time': 3, 'burst_time': 6}
            ]

    return processes


def display_input_summary(processes):
    """显示输入进程的摘要信息"""
    print("\n" + "=" * 50)
    print("输入的进程信息摘要")
    print("=" * 50)
    print("进程ID\t到达时间\t执行时间")
    print("-" * 30)

    for process in processes:
        print(f"{process['process_id']}\t\t{process['arrival_time']}\t\t{process['burst_time']}")

    print("-" * 30)
    print(f"总进程数: {len(processes)}")
    print("=" * 50)


# 主程序
if __name__ == "__main__":
    print("先来先服务调度算法模拟程序")
    print("=" * 50)

    # 获取用户输入
    processes = get_user_input()

    # 显示输入摘要
    display_input_summary(processes)

    # 执行FCFS调度算法
    print("\n开始执行先来先服务调度算法...")
    results = fcfs(processes)

    # 显示最终结果
    print_results(results)

    # 等待用户按键退出
    input("\n按回车键退出程序...")

