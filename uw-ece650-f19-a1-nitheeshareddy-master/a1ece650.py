import re

class Line(object):
    def __init__(self, src, dest):
        self.src = src
        self.dest = dest

    def __repr__(self):
        return repr(self.src) + ' --> ' + repr(self.dest)

    def __str__(self):
        return str(self.src) + '-->' + str(self.dest)


class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '({0:.2f}, {1:.2f})'.format(self.x, self.y)

    def __str__(self):
        return repr(self)


def intersection(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dest.x, l1.dest.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dest.x, l2.dest.y

    xnumerator = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    xdenominator = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))

    ynumerator = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    ydenominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

    if xnumerator == -0.0:
        xnumerator = 0.0

    if ynumerator == -0.0:
        ynumerator = 0.0

    if xdenominator != 0 and ydenominator != 0:
        xcoord = xnumerator / xdenominator
        ycoord = ynumerator / ydenominator
        if segment(x1, x2, xcoord, y1, y2, ycoord) and segment(x3, x4, xcoord, y3, y4, ycoord):
            if xcoord == -0.0:
                xcoord = 0.0
            if ycoord == -0.0:
                ycoord = 0.0

            return (xcoord, ycoord)

    if xdenominator == 0 and ydenominator == 0:
        xden1 = (x3 - x1) * (y2 - y4) - (x2 - x4) * (y3 - y1)
        xden2 = (x4 - x1) * (y3 - y2) - (x3 - x2) * (y4 - y1)

        if xden1 == 0 and xden2 == 0:
            return 'parallel'

    else:
        return None


def validateStreetName(n):
    if n == "":
        return False
    if all(x.isalpha() or x.isspace() for x in n):
        return True
    return False


def validateNumberInsideBrackets(b):
    for segment in re.findall("[(][^)]*[)]", b):
        if len(re.findall("[-+]?\d+[\.]?\d*[eE]?[-+]?\d*", segment)) == 2:
            continue
        else:
            return False
    return True


def validateInputBrackets(b):
    if "(" not in b or ")" not in b:
        return False
    count = 0
    for i in b:
        if i == "(":
            count += 1
        elif i == ")":
            if count == 0:
                return False
            count -= 1
    return count == 0


def getAllDataPoints(p):
    updatedvertex = []

    p = p.replace(" ", "")
    p = p.replace(")(", ",")
    p = p.replace(")", "")
    p = p.replace("(", "")
    p = p.replace(",", " ")
    p = p.split(' ')
    p_updated = [x for x in p if x != ""]
    evenvertex = p_updated[0::2]
    oddvertex = p_updated[1::2]
    for i in range(len(evenvertex)):
        updatedvertex.append((float(evenvertex[i]), float(oddvertex[i])))
    return updatedvertex


def segment(x1, x2, xcoord, y1, y2, ycoord):
    if min(x1, x2) <= xcoord and xcoord <= max(x1, x2) and min(y1, y2) <= ycoord and ycoord <= max(y1, y2):
        return True
    else:
        return False

dict = {}
vertex_index = []


def generateVertexAndEdges():
    inputDict = dict

    all_data_points = []
    for i in inputDict:
        all_data_points.append(inputDict[i])

    intersectln = []
    for i in range(len(all_data_points)):
        for j in range(i + 1, len(all_data_points)):
            for m in range(len(all_data_points[i]) - 1):
                line1 = Line(Point(all_data_points[i][m][0], all_data_points[i][m][1]),
                          Point(all_data_points[i][m + 1][0], all_data_points[i][m + 1][1]))
                for n in range(len(all_data_points[j]) - 1):
                    line2 = Line(Point(all_data_points[j][n][0], all_data_points[j][n][1]),
                              Point(all_data_points[j][n + 1][0], all_data_points[j][n + 1][1]))
                    intersectionresult = intersection(line1, line2)
                    if intersectionresult and intersectionresult != 'parallel':
                        if [all_data_points[i][m], intersection(line1, line2), all_data_points[i][m + 1]] \
                                not in intersectln:
                            intersectln.append([all_data_points[i][m], intersection(line1, line2),
                                                all_data_points[i][m + 1]])
                        if [all_data_points[j][n], intersection(line1, line2), all_data_points[j][n + 1]] \
                                not in intersectln:
                            intersectln.append([all_data_points[j][n], intersection(line1, line2),
                                                all_data_points[j][n + 1]])
                        if intersectionresult not in vertex_index:
                            vertex_index.append(intersectionresult)
                    elif intersectionresult == 'parallel':
                        line3 = Line(Point(all_data_points[i][m][0], all_data_points[i][m][1]),
                                  Point(all_data_points[j][n][0], all_data_points[j][n][1]))
                        line4 = Line(Point(all_data_points[i][m + 1][0], all_data_points[i][m + 1][1]),
                                  Point(all_data_points[j][n + 1][0], all_data_points[j][n + 1][1]))
                        if intersection(line3, line4) == "parallel":
                            points = [all_data_points[i][m], all_data_points[i][m + 1], all_data_points[j][n],
                                        all_data_points[j][n + 1]]
                            points = sorted(points)
                            intersectln.append([points[0], points[1], points[2], points[3]])
                    else:
                        continue
    new_intersection_line = []
    temp = []
    for j in range(len(intersectln) - 1):
        for i in range(j + 1, len(intersectln)):

            if intersectln[j][0] == intersectln[i][0] and intersectln[j][-1] == intersectln[i][-1]:
                for a in intersectln[j]:
                    temp.append(a)
                for b in intersectln[i]:
                    if b not in temp:
                        temp.append(b)
                    else:
                        continue
                del intersectln[i]
                del intersectln[j]
                new_intersection_line.append(sorted(temp))
                temp = []
                break
        if j == len(intersectln) - 2:
            break
    for i in new_intersection_line:
        intersectln.append(i)
    position = []
    for i in intersectln:
        for j in i:
            if j not in position:
                position.append(j)
    vertex_output = {}
    for i in position:
        vertex_output[vertex_index.index(i)] = i
    print "V = {"
    for key, value in vertex_output.items():
        print ' ' + str(key) + ': ' + '(' + str(round(value[0], 2)) + ',' + str(round(value[1], 2)) + ')'
    print "}"
    edges = []
    for i in range(len(intersectln)):
        for j in range(len(intersectln[i]) - 1):
            line_1 = list(vertex_output.keys())[list(vertex_output.values()).index(intersectln[i][j])]
            line_2 = list(vertex_output.keys())[list(vertex_output.values()).index(intersectln[i][j + 1])]
            if line_1 != line_2:
                result = (line_1, line_2) if line_1 < line_2 else (line_2, line_1)
                edges.append(result)
    edges = list(set(edges))
    print "E = {"
    for i in range(len(edges)):
        if i == len(edges) - 1:
            print ' <' + str(edges[i][0]) + ',' + str(edges[i][1]) + '>'
        else:
            print ' <' + str(edges[i][0]) + ',' + str(edges[i][1]) + '>,'

    print '}'


def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment

    while True:
        try:
            input_data=raw_input()
        except EOFError:
            break


        if 'g' == input_data.strip(' '):
            generateVertexAndEdges()
        elif '"' not in input_data:
            print "Error: Please enter a valid input!"
        elif '"' in input_data and len(input_data.split('"')) == 3:
            input_lst = input_data.split('"')
            if validateInputBrackets(input_lst[2]) and validateNumberInsideBrackets(input_lst[2]) and \
                    input_lst[2].split(' ')[0] == "" and validateStreetName(input_lst[1]):
                vertex = getAllDataPoints(input_lst[2])
                for i in vertex:
                    if i not in vertex_index:
                        vertex_index.append(i)
                if 'a' == input_lst[0].strip(' ') and len(input_lst) == 3 and 'a ' in input_lst[0]:
                    if input_lst[1].lower() not in dict:
                        dict[input_lst[1].lower()] = vertex
                    else:
                        print "Error: The street already exists.Please use command 'c' for updating information"
                elif 'c' in input_lst[0].strip(' ') and len(input_lst) == 3 and 'c ' in input_lst[0]:
                    if input_lst[1].lower() in dict:
                        dict[input_lst[1].lower()] = vertex
                    else:
                        print "Error: 'c' cmd provided for the street do not exist!"
                else:
                    print "Error: Invalid command!Please check again!"
            elif 'r' in input_lst[0].strip(' ') and validateStreetName(input_lst[1]) and len(
                  input_lst[0]) == 2 and 'r ' in input_lst[0]:
                if input_lst[1].lower() in dict:
                    del dict[input_lst[1].lower()]
                else:
                    print "Error: 'r' cmd provided for the street do not exist!"
            else:
                print "Error: Entered invalid input!Please check again!"
        else:
            print "Error: Provided format is invalid, please check once again!"


if __name__ == '__main__':
    main()