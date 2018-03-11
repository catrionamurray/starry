/**
Tabulated square roots for integers.

*/

#ifndef _STARRY_SQRTINT_H_
#define _STARRY_SQRTINT_H_

#include <cmath>
#include <iostream>

namespace sqrtint {

#define STARRY_MAX_SQRT 201

    // Table of sqrt(n) for nonnegative integer n
    //
    // Mathematica:
    //      Table[{StringPadRight[ToString[n],4],
    //             StringPadRight[ToString[FortranForm[
    //                  SetPrecision[Sqrt[n],25]]],30]},{n,0,201}]
    //
    static struct {int n; double f;} sqrt_table[] = {
        {0   ,   0                             },
        {1   ,   1.                            },
        {2   ,   1.4142135623730950488016887   },
        {3   ,   1.7320508075688772935274463   },
        {4   ,   2.                            },
        {5   ,   2.236067977499789696409174    },
        {6   ,   2.449489742783178098197284    },
        {7   ,   2.645751311064590590501616    },
        {8   ,   2.828427124746190097603377    },
        {9   ,   3.                            },
        {10  ,   3.162277660168379331998894    },
        {11  ,   3.316624790355399849114933    },
        {12  ,   3.464101615137754587054893    },
        {13  ,   3.605551275463989293119221    },
        {14  ,   3.741657386773941385583749    },
        {15  ,   3.872983346207416885179265    },
        {16  ,   4.                            },
        {17  ,   4.12310562561766054982141     },
        {18  ,   4.242640687119285146405066    },
        {19  ,   4.358898943540673552236982    },
        {20  ,   4.472135954999579392818347    },
        {21  ,   4.582575694955840006588047    },
        {22  ,   4.69041575982342955456563     },
        {23  ,   4.795831523312719541597438    },
        {24  ,   4.898979485566356196394568    },
        {25  ,   5.                            },
        {26  ,   5.099019513592784830028224    },
        {27  ,   5.196152422706631880582339    },
        {28  ,   5.291502622129181181003232    },
        {29  ,   5.38516480713450403125071     },
        {30  ,   5.477225575051661134569698    },
        {31  ,   5.567764362830021922119471    },
        {32  ,   5.656854249492380195206755    },
        {33  ,   5.744562646538028659850611    },
        {34  ,   5.830951894845300470874153    },
        {35  ,   5.916079783099616042567328    },
        {36  ,   6.                            },
        {37  ,   6.082762530298219688999684    },
        {38  ,   6.164414002968976450250192    },
        {39  ,   6.244997998398398205846893    },
        {40  ,   6.324555320336758663997787    },
        {41  ,   6.403124237432848686488218    },
        {42  ,   6.480740698407860230965967    },
        {43  ,   6.55743852430200065234411     },
        {44  ,   6.633249580710799698229865    },
        {45  ,   6.70820393249936908922752     },
        {46  ,   6.782329983125268139064556    },
        {47  ,   6.855654600401044124935871    },
        {48  ,   6.92820323027550917410979     },
        {49  ,   7.                            },
        {50  ,   7.07106781186547524400844     },
        {51  ,   7.1414284285428499979994      },
        {52  ,   7.21110255092797858623844     },
        {53  ,   7.280109889280518271097302    },
        {54  ,   7.34846922834953429459185     },
        {55  ,   7.416198487095662948711397    },
        {56  ,   7.4833147735478827711675      },
        {57  ,   7.549834435270749697236685    },
        {58  ,   7.615773105863908285661411    },
        {59  ,   7.681145747868608175769687    },
        {60  ,   7.74596669241483377035853     },
        {61  ,   7.810249675906654394129723    },
        {62  ,   7.874007874011811019685034    },
        {63  ,   7.93725393319377177150485     },
        {64  ,   8.                            },
        {65  ,   8.062257748298549652366613    },
        {66  ,   8.124038404635960360459884    },
        {67  ,   8.185352771872449969953704    },
        {68  ,   8.24621125123532109964282     },
        {69  ,   8.306623862918074852584263    },
        {70  ,   8.36660026534075547978172     },
        {71  ,   8.42614977317635863063414     },
        {72  ,   8.48528137423857029281013     },
        {73  ,   8.544003745317531167871648    },
        {74  ,   8.602325267042626771729474    },
        {75  ,   8.66025403784438646763723     },
        {76  ,   8.71779788708134710447396     },
        {77  ,   8.774964387392122060406388    },
        {78  ,   8.831760866327846854764043    },
        {79  ,   8.888194417315588850091442    },
        {80  ,   8.94427190999915878563669     },
        {81  ,   9.                            },
        {82  ,   9.055385138137416626573808    },
        {83  ,   9.110433579144298881945626    },
        {84  ,   9.16515138991168001317609     },
        {85  ,   9.219544457292887310002274    },
        {86  ,   9.273618495495703752516416    },
        {87  ,   9.327379053088815045554476    },
        {88  ,   9.38083151964685910913126     },
        {89  ,   9.43398113205660381132066     },
        {90  ,   9.48683298050513799599668     },
        {91  ,   9.539392014169456491526216    },
        {92  ,   9.59166304662543908319488     },
        {93  ,   9.643650760992954995760031    },
        {94  ,   9.695359714832658028148881    },
        {95  ,   9.746794344808963906838413    },
        {96  ,   9.79795897113271239278914     },
        {97  ,   9.848857801796104721746211    },
        {98  ,   9.89949493661166534161182     },
        {99  ,   9.9498743710661995473448      },
        {100 ,   10.                           },
        {101 ,   10.049875621120890270219265   },
        {102 ,   10.099504938362077953363386   },
        {103 ,   10.14889156509221946864852    },
        {104 ,   10.19803902718556966005645    },
        {105 ,   10.246950765959598383221039   },
        {106 ,   10.295630140987000315797369   },
        {107 ,   10.344080432788600469738599   },
        {108 ,   10.39230484541326376116468    },
        {109 ,   10.440306508910550179757754   },
        {110 ,   10.488088481701515469914535   },
        {111 ,   10.535653752852738848401405   },
        {112 ,   10.58300524425836236200646    },
        {113 ,   10.630145812734649407999122   },
        {114 ,   10.677078252031311210811524   },
        {115 ,   10.72380529476360830481416    },
        {116 ,   10.77032961426900806250142    },
        {117 ,   10.81665382639196787935766    },
        {118 ,   10.862780491200215723891493   },
        {119 ,   10.908712114635714411502154   },
        {120 ,   10.9544511501033222691394     },
        {121 ,   11.                           },
        {122 ,   11.045361017187260774210914   },
        {123 ,   11.0905365064094171620516     },
        {124 ,   11.13552872566004384423894    },
        {125 ,   11.18033988749894848204587    },
        {126 ,   11.22497216032182415675125    },
        {127 ,   11.269427669584644882850004   },
        {128 ,   11.31370849898476039041351    },
        {129 ,   11.357816691600547221784676   },
        {130 ,   11.40175425099137979136049    },
        {131 ,   11.445523142259597039043434   },
        {132 ,   11.48912529307605731970122    },
        {133 ,   11.532562594670795889354183   },
        {134 ,   11.57583690279022547355581    },
        {135 ,   11.6189500386222506555378     },
        {136 ,   11.66190378969060094174831    },
        {137 ,   11.704699910719625109102098   },
        {138 ,   11.747340124470730586968506   },
        {139 ,   11.789826122551595968469184   },
        {140 ,   11.83215956619923208513466    },
        {141 ,   11.874342087037917234672918   },
        {142 ,   11.916375287812984954052121   },
        {143 ,   11.958260743101398021129841   },
        {144 ,   12.                           },
        {145 ,   12.041594578792295480128241   },
        {146 ,   12.083045973594572068282839   },
        {147 ,   12.12435565298214105469212    },
        {148 ,   12.16552506059643937799937    },
        {149 ,   12.206555615733702951897855   },
        {150 ,   12.24744871391589049098642    },
        {151 ,   12.288205727444507591812164   },
        {152 ,   12.32882800593795290050038    },
        {153 ,   12.36931687685298164946423    },
        {154 ,   12.409673645990856596133242   },
        {155 ,   12.449899597988732374835971   },
        {156 ,   12.48999599679679641169379    },
        {157 ,   12.529964086141667788495365   },
        {158 ,   12.569805089976534715702559   },
        {159 ,   12.609520212918491531228626   },
        {160 ,   12.64911064067351732799557    },
        {161 ,   12.688577540449520380193773   },
        {162 ,   12.7279220613578554392152     },
        {163 ,   12.767145334803704661710952   },
        {164 ,   12.80624847486569737297644    },
        {165 ,   12.845232578665129020116898   },
        {166 ,   12.884098726725125494644327   },
        {167 ,   12.922847983320085468844811   },
        {168 ,   12.96148139681572046193193    },
        {169 ,   13.                           },
        {170 ,   13.038404810405297429165943   },
        {171 ,   13.07669683062202065671095    },
        {172 ,   13.11487704860400130468822    },
        {173 ,   13.152946437965905439962487   },
        {174 ,   13.190905958272919170936808   },
        {175 ,   13.22875655532295295250808    },
        {176 ,   13.26649916142159939645973    },
        {177 ,   13.304134695650070725046031   },
        {178 ,   13.341664064126333712489436   },
        {179 ,   13.379088160259652015026881   },
        {180 ,   13.41640786499873817845504    },
        {181 ,   13.453624047073710317163085   },
        {182 ,   13.490737563232041465550306   },
        {183 ,   13.527749258468682897425875   },
        {184 ,   13.56465996625053627812911    },
        {185 ,   13.601470508735443345029937   },
        {186 ,   13.638181696985855892758598   },
        {187 ,   13.674794331177343158572165   },
        {188 ,   13.71130920080208824987174    },
        {189 ,   13.74772708486752001976414    },
        {190 ,   13.784048752090221767955913   },
        {191 ,   13.820274961085253313904481   },
        {192 ,   13.85640646055101834821957    },
        {193 ,   13.892443989449804508432547   },
        {194 ,   13.928388277184119338467739   },
        {195 ,   13.964240043768941169883925   },
        {196 ,   14.                           },
        {197 ,   14.035668847618199630519461   },
        {198 ,   14.07124727947028866369689    },
        {199 ,   14.106735979665884425232164   },
        {200 ,   14.14213562373095048801689    },
        {201 ,   14.177446878757825202955619   }
};

    // Table of 1 / sqrt(n) for nonnegative integer n
    //
    // Mathematica:
    //      Table[{StringPadRight[ToString[n],4],
    //             StringPadRight[ToString[FortranForm[
    //                  SetPrecision[1./Sqrt[n],25]]],30]},{n,0,201}]
    //
    static struct {int n; double f;} invsqrt_table[] = {
        {0   ,   INFINITY                      },
        {1   ,   1.                            },
        {2   ,   0.7071067811865474617150085   },
        {3   ,   0.5773502691896258420811705   },
        {4   ,   0.5                           },
        {5   ,   0.4472135954999579277036048   },
        {6   ,   0.4082482904638630727411908   },
        {7   ,   0.3779644730092271975863127   },
        {8   ,   0.3535533905932737308575042   },
        {9   ,   0.3333333333333333148296163   },
        {10  ,   0.3162277660168379411764761   },
        {11  ,   0.3015113445777636291822432   },
        {12  ,   0.2886751345948129210405853   },
        {13  ,   0.2773500981126145736865851   },
        {14  ,   0.2672612419124243965384835   },
        {15  ,   0.2581988897471610977518708   },
        {16  ,   0.25                          },
        {17  ,   0.2425356250363329690955538   },
        {18  ,   0.235702260395515811319811    },
        {19  ,   0.2294157338705617443785911   },
        {20  ,   0.2236067977499789638518024   },
        {21  ,   0.218217890235992389991182    },
        {22  ,   0.2132007163556104145651915   },
        {23  ,   0.208514414057074770614264    },
        {24  ,   0.2041241452319315363705954   },
        {25  ,   0.2000000000000000111022302   },
        {26  ,   0.1961161351381840445284865   },
        {27  ,   0.1924500897298752621900064   },
        {28  ,   0.1889822365046135987931564   },
        {29  ,   0.1856953381770518607396525   },
        {30  ,   0.1825741858350553581402664   },
        {31  ,   0.1796053020267749100735699   },
        {32  ,   0.1767766952966368654287521   },
        {33  ,   0.1740776559556978519971437   },
        {34  ,   0.1714985851425088192456769   },
        {35  ,   0.1690308509457033037115536   },
        {36  ,   0.1666666666666666574148081   },
        {37  ,   0.1643989873053572914329834   },
        {38  ,   0.1622214211307625442159974   },
        {39  ,   0.1601281538050871311451573   },
        {40  ,   0.158113883008418970588238    },
        {41  ,   0.1561737618886060718903508   },
        {42  ,   0.1543033499620919124506457   },
        {43  ,   0.1524985703326046659888959   },
        {44  ,   0.1507556722888818145911216   },
        {45  ,   0.1490711984999859573974845   },
        {46  ,   0.1474419561548971358000415   },
        {47  ,   0.1458649914978945583143854   },
        {48  ,   0.1443375672974064605202926   },
        {49  ,   0.1428571428571428492126927   },
        {50  ,   0.1414213562373095034452319   },
        {51  ,   0.1400280084028009686125671   },
        {52  ,   0.1386750490563072868432926   },
        {53  ,   0.1373605639486890350742243   },
        {54  ,   0.1360827634879543390766798   },
        {55  ,   0.1348399724926484244402758   },
        {56  ,   0.1336306209562121982692418   },
        {57  ,   0.1324532357065043852717423   },
        {58  ,   0.1313064328597225438599594   },
        {59  ,   0.1301889109808238853549511   },
        {60  ,   0.1290994448735805488759354   },
        {61  ,   0.128036879932895975020557    },
        {62  ,   0.1270001270001905047113411   },
        {63  ,   0.125988157669742389943579    },
        {64  ,   0.125                         },
        {65  ,   0.1240347345892084673923961   },
        {66  ,   0.1230914909793327238762828   },
        {67  ,   0.1221694443563052234758004   },
        {68  ,   0.1212678125181664845477769   },
        {69  ,   0.120385853085769198300703    },
        {70  ,   0.1195228609334393621299597   },
        {71  ,   0.1186781658193853256078043   },
        {72  ,   0.1178511301977579056599055   },
        {73  ,   0.1170411471961305682754073   },
        {74  ,   0.1162476387438192843015727   },
        {75  ,   0.1154700538379251767429068   },
        {76  ,   0.1147078669352808721892956   },
        {77  ,   0.1139605764596379466890852   },
        {78  ,   0.1132277034144595628806229   },
        {79  ,   0.1125087900926023953829258   },
        {80  ,   0.1118033988749894819259012   },
        {81  ,   0.1111111111111111049432054   },
        {82  ,   0.110431526074846533824747    },
        {83  ,   0.1097642599896903464751219   },
        {84  ,   0.109108945117996194995591    },
        {85  ,   0.1084652289093280819187726   },
        {86  ,   0.1078327732034384117687509   },
        {87  ,   0.1072112534837794811659606   },
        {88  ,   0.1066003581778052072825957   },
        {89  ,   0.1059997880006360027049794   },
        {90  ,   0.1054092553389459757662294   },
        {91  ,   0.104828483672191830056164    },
        {92  ,   0.104257207028537385307132    },
        {93  ,   0.1036951694730425255208672   },
        {94  ,   0.1031421246258793361638695   },
        {95  ,   0.1025978352085154116091203   },
        {96  ,   0.1020620726159657681852977   },
        {97  ,   0.1015346165133619166232037   },
        {98  ,   0.1010152544552210540640402   },
        {99  ,   0.1005037815259212097274144   },
        {100 ,   0.1000000000000000055511151   },
        {101 ,   0.09950371902099891530113496  },
        {102 ,   0.09901475429766744273951673  },
        {103 ,   0.09853292781642931930097262  },
        {104 ,   0.09805806756909202226424327  },
        {105 ,   0.09759000729485332870094538  },
        {106 ,   0.09712858623572641347809764  },
        {107 ,   0.09667364890456635329041291  },
        {108 ,   0.09622504486493763109500321  },
        {109 ,   0.09578262852211513744826732  },
        {110 ,   0.09534625892455923790169692  },
        {111 ,   0.09491579957524989841211038  },
        {112 ,   0.09449111825230679939657819  },
        {113 ,   0.0940720868383597280670827   },
        {114 ,   0.09365858115816939888365056  },
        {115 ,   0.09325048082403138149931721  },
        {116 ,   0.09284766908852593036982626  },
        {117 ,   0.09245003270420484864366983  },
        {118 ,   0.09205746178983234551118642  },
        {119 ,   0.09166984970282113232009635  },
        {120 ,   0.09128709291752767907013322  },
        {121 ,   0.09090909090909091161414324  },
        {122 ,   0.0905357460425185306407414   },
        {123 ,   0.09016696346674323014891428  },
        {124 ,   0.08980265101338745503678496  },
        {125 ,   0.08944271909999158831627852  },
        {126 ,   0.0890870806374747942202319   },
        {127 ,   0.08873565094161138533390698  },
        {128 ,   0.08838834764831843271437606  },
        {129 ,   0.08804509063256238388106567  },
        {130 ,   0.08770580193070293129053994  },
        {131 ,   0.08737040566610379532797026  },
        {132 ,   0.08703882797784892599857187  },
        {133 ,   0.08671099695241199500905793  },
        {134 ,   0.0863868425581360149800858   },
        {135 ,   0.08606629658238702795802766  },
        {136 ,   0.08574929257125440962283847  },
        {137 ,   0.08543576577167609487073463  },
        {138 ,   0.08512565307587485752272016  },
        {139 ,   0.08481889296799709210272766  },
        {140 ,   0.08451542547285165185577682  },
        {141 ,   0.08421519210665190380282752  },
        {142 ,   0.08391813582966890783243485  },
        {143 ,   0.08362420100070908079992194  },
        {144 ,   0.08333333333333332870740406  },
        {145 ,   0.083045479853739972986304    },
        {146 ,   0.08276058886023679450616441  },
        {147 ,   0.08247860988423226513699404  },
        {148 ,   0.0821994936526786457164917   },
        {149 ,   0.08192319205190405628780326  },
        {150 ,   0.08164965809277262287491084  },
        {151 ,   0.08137884587711594064352028  },
        {152 ,   0.08111071056538127210799871  },
        {153 ,   0.08084520834544431377999274  },
        {154 ,   0.08058229640253802839033881  },
        {155 ,   0.08032193289024988636093383  },
        {156 ,   0.08006407690254356557257864  },
        {157 ,   0.07980868844676221318668041  },
        {158 ,   0.07955572841757299618059562  },
        {159 ,   0.07930515857181441585765924  },
        {160 ,   0.07905694150420948529411902  },
        {161 ,   0.07881104062391006137566762  },
        {162 ,   0.07856742013183860839919959  },
        {163 ,   0.07832604499879573800580346  },
        {164 ,   0.0780868809443030359451754   },
        {165 ,   0.07784989441615229599502612  },
        {166 ,   0.07761505257063328055977536  },
        {167 ,   0.07738232325341368178506229  },
        {168 ,   0.07715167498104595622532287  },
        {169 ,   0.07692307692307692734701163  },
        {170 ,   0.07669649888473703547298044  },
        {171 ,   0.0764719112901872388743385   },
        {172 ,   0.07624928516630233299444797  },
        {173 ,   0.07602859212697055113494571  },
        {174 ,   0.07580980435789033688998018  },
        {175 ,   0.07559289460184544229282011  },
        {176 ,   0.07537783614444090729556081  },
        {177 ,   0.0751646028002828925362877   },
        {178 ,   0.07495316889958614225974998  },
        {179 ,   0.07474350927519358978035058  },
        {180 ,   0.07453559924999297869874226  },
        {181 ,   0.07432941462471663596200955  },
        {182 ,   0.07412493166611011652378238  },
        {183 ,   0.07392212709545728510640572  },
        {184 ,   0.07372097807744856790002075  },
        {185 ,   0.07352146220938077236795039  },
        {186 ,   0.07332355751067665128317685  },
        {187 ,   0.0731272424127130671323016   },
        {188 ,   0.07293249574894727915719272  },
        {189 ,   0.07273929674533079203779806  },
        {190 ,   0.07254762501100116278696106  },
        {191 ,   0.07235746052924216242807631  },
        {192 ,   0.07216878364870323026014631  },
        {193 ,   0.07198157507486944994834488  },
        {194 ,   0.07179581586177381791191721  },
        {195 ,   0.07161148740394329692549036  },
        {196 ,   0.07142857142857142460634634  },
        {197 ,   0.07124704998790964971533413  },
        {198 ,   0.07106690545187013818839716  },
        {199 ,   0.07088812050083358184959792  },
        {200 ,   0.07071067811865475172261597  },
        {201 ,   0.07053456158585982849196228  }
    };

    // Square root of n
    double sqrt_int(int n) {
        if (n < 0) {
            // TODO: Better error handling
            std::cout << "ERROR: Argument of sqrt_int must be nonnegative." << std::endl;
            return NAN;
        } else if (n > STARRY_MAX_SQRT) {
            return sqrt(n);
        } else {
            return sqrt_table[n].f;
        }
    }

    // Inverse of the square root of n
    double invsqrt_int(int n) {
        if (n < 0) {
            // TODO: Better error handling
            std::cout << "ERROR: Argument of invsqrt_int must be nonnegative." << std::endl;
            return NAN;
        } else if (n > STARRY_MAX_SQRT) {
            return 1. / sqrt(n);
        } else {
            return invsqrt_table[n].f;
        }
    }

}; // namespace sqrtint

#endif
